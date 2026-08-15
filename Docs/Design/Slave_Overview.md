# Slave firmware flow diagram

## Connection Block Diagram:

```mermaid
graph TD
    User([User / Operator]) -->|Sends SMS Command| GSM[GSM Network / Cellular Tower]
    
    subgraph Central Master Controller Station
        GSM -->|RF Signal| Ant[GSM Antenna]
        Ant --> SIM[GSM Module<br>SIM800/SIM7600]
        SIM -->|UART Serial Communication| MN[Combined Master Node<br>ESP32 MCU]
        MN -->|SPI Bus Interface| LoRaM[Master LoRa Transceiver]
    end

    LoRaM -->|Long-Range Wireless RF Signal| LoRaS[Subordinate LoRa Transceiver]
    
    subgraph Remote Field Actuator Site
        LoRaS -->|SPI Bus Interface| SN[Subordinate Node<br>ESP32 MCU]
        SN -->|GPIO Control Output| Relay[Relay Driver Circuit]
        Relay -->|Electrical Pulse / Power| Valve[Water / Liquid Valve]
    end

    %% Visual Styling
    classDef mcu fill:#e1f5fe,stroke:#01579b,stroke-width:2px,color:#000;
    classDef RF fill:#fff3e0,stroke:#ef6c00,stroke-width:2px,color:#000;
    classDef mechanical fill:#f3e5f5,stroke:#4a148c,stroke-width:2px,color:#000;
    classDef user fill:#e8f5e9,stroke:#2e7d32,stroke-width:2px,color:#000;
    
    class MN,SN mcu;
    class GSM,Ant,SIM,LoRaM,LoRaS RF;
    class Relay,Valve mechanical;
    class User user;
```

## 1. Flow Overview:

```mermaid
flowchart TD
    A[Loop Start]
    B[Expect Order]
    C[Parse Order]
    D[Apply Order]
    E[Update Master]

    A --> B
    B --> C
    C --> D
    D --> E
    E -->|Next Command| B
```

## 2. Expect Order Overview:

```mermaid
flowchart TD
    A[Expect Order Start]
    B[Wait For Order]
    C[Evaluate The Integrity of Order]
    D[AckwnoledgeOrder]
    E[SetError]
    F[OrderProcessed]

    A --> B
    B --> G{Is Sucessful?}
    G -->|Yes| C
    G -->|No| E
    C --> H{Is Sucessful?} 
    H -->|Yes| D
    H -->|No| E
    D --> I{Is Successful?}
    I -->|Yes| F
    I -->|No| E
```

## 3. Parse Order Overview:

```mermaid
flowchart TD
    A[Parse Order Start]
    B{Is Error Set}
    C[Parse Request]
    D{Is Successful?}
    E[Validate The Request]
    F{Is Values Acceptable}
    G[Update the Global info]
    H[SetError]
    I[OrderProcessed]

    A --> B
    B -->|No| C
    B -->|Yes| H
    C --> D{Is Sucessful?} 
    D -->|Yes| E
    D -->|No| H
    E --> F{Is Successful?}
    F -->|Yes| G
    F -->|No| H
    G --> I
```

## 4. Apply Order Overview:

```mermaid
flowchart TD
    A[Apply Order Start]
    B{Is Error Set}
    C{Is the value new}
    D[Actuate The Valves]
    E{Is Successful?}
    F[Update the internal struct]
    G{Timer update needed?}
    H[up/reset timer]
    I{Is Successful?}
    J[ValuesApplied]
    K[SetError]

    A --> B
    B -->|No| C
    C -->|Yes| D 
    D --> E
    E -->|Yes| F
    E -->|No| K
    B -->|Yes| K
    F --> G
    C -->|No| G
    G -->|Yes| H
    H --> I
    I -->|Yes| J
    I -->|No| K
    G --->|No| J
```

## 5. Update Master Overview:

```mermaid
flowchart TD
    A[Update Master Start]
    B{Is Error Set}
    C[Extract Error Discription and note]
    D[Append Success Discription]
    E[Form tx frame with Current Slave state and Operation Status]
    F[Transmit to master]
    G{Is Successful?}
    H[Set the failure count]
    I[Log the status]
    J[Clear the error]
    K[End]

    A --> B
    B -->|Yes| C
    C --> E
    B -->|No| D
    D --> E
    E --> F
    F --> G
    G -->|Yes| J
    J --> K
    G -->|No| H
    H --> I
    I --> J
```