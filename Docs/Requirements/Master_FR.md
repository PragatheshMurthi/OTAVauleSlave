# Master Node Functional Requirements

## Document Information

| Item | Description |
|--------|------------|
| Feature | LoRa Master Node Firmware |
| Platform | ESP32 + WAN |
| Supported WAN | WiFi,GSM,LoRa |
| Version | 1.0 |
| Status | Draft |
| Owner | P&B Company |

## 1 Command Reception

### FR-MASTER-001: WAN Command Reception

The Master Node shall receive commands from an external WAN interface.
Only one interface may be active at a time, or multiple interfaces may be supported depending on the final system architecture.

### Acceptance Criteria

- Commands shall be accepted from supported WAN interfaces.
- Command integrity shall be validated.
- Invalid commands shall be rejected and reported.

---

## 2 Command Processing

### FR-MASTER-002: Command Parsing and Routing

The Master Node shall parse incoming commands and separate them into:

- Master-specific commands
- Slave-specific commands

### Acceptance Criteria

- Commands intended for the Master shall be processed locally.
- Commands intended for Slaves shall be routed to the respective Slave Nodes.
- Multiple Slave commands shall be supported in a single request.

---

## 3 Slave Command Delivery

### FR-MASTER-003: Reliable Slave Command Transmission

The Master Node shall continuously transmit commands to the target Slave until a terminal response is received.

### Terminal Responses

- Success
- Failure

### Acceptance Criteria

- Command retransmission shall continue until a response is received.
- Retransmission shall stop immediately after a Success response.
- Retransmission shall stop immediately after a Failure response.
- Retransmission for other pending Slaves shall continue independently.

---

### FR-MASTER-004: Parallel Slave Command Management

The Master Node shall independently manage command delivery status for multiple Slaves.

### Acceptance Criteria

- Success or failure of one Slave shall not affect communication with other Slaves.
- Pending Slaves shall continue receiving retransmissions.
- Communication state shall be maintained per Slave.

---

## 4 Motor Control Coordination

### FR-MASTER-005: Motor ON Interlock

The Master Node shall not activate the motor until all requested Slave operations have completed successfully.

### Acceptance Criteria

- All targeted Slaves must acknowledge successful operation.
- If any Slave reports failure, motor activation shall be prevented.
- Failure reason shall be reported to WAN.

### Example

```text
Command Received:
- Open Slave 1
- Open Slave 2
- Open Slave 3
- Motor ON

Result:
Slave 1 = Success
Slave 2 = Success
Slave 3 = Failure

Motor ON = Blocked
```

---

### FR-MASTER-006: Motor ON Execution

The Master Node shall activate the motor only when all prerequisite Slave operations are successful.

### Acceptance Criteria

- All required Slave acknowledgements received.
- No pending Slave operation exists.
- No Slave has reported failure.
- Motor activation command executed.

---

## 5 Command Prioritization

### FR-MASTER-007: Motor OFF Priority Handling

The Master Node shall prioritize Motor OFF commands over all other pending operations.

### Acceptance Criteria

- Motor OFF command shall immediately enter execution queue.
- Ongoing lower-priority tasks may be interrupted if required.
- Motor OFF execution latency shall be minimized.

### Priority Order

```text
Priority 1:
Motor OFF

Priority 2:
Emergency Commands

Priority 3:
Slave Control Commands

Priority 4:
Periodic Status Update
```

---

## 6 Response Handling

### FR-MASTER-008: WAN Response Reporting

The Master Node shall report command execution results back to WAN.

### Response Sources

- Master operations
- Motor operations
- Slave operations

### Acceptance Criteria

- Positive acknowledgements shall be reported.
- Negative acknowledgements shall be reported.
- Failure reason shall be included where applicable.

---

## 7 Periodic Slave Monitoring

### FR-MASTER-009: Periodic Slave Health Monitoring

The Master Node shall periodically collect operational data from all Slaves during active operating hours.

### Collected Parameters

- Slave availability
- Battery level
- Battery zone
- Current operating status
- Latest operation result
- Signal strength

### Acceptance Criteria

- Data polling interval shall be configurable.
- Failed polling attempts shall be logged.
- Monitoring shall only occur during active working hours.

---

### FR-MASTER-010: WAN Status Synchronization

The Master Node shall periodically upload collected Slave information to WAN.

### Acceptance Criteria

- Status updates shall be transmitted at configurable intervals.
- Communication failures shall be logged.
- Retransmission mechanism should be supported.

---

## 8 Motor Health Monitoring

### FR-MASTER-011: Motor Health Check

The Master Node shall perform a motor health evaluation before or during motor operation.

### Initial Trigger

- One health check per motor activation cycle.

### Acceptance Criteria

- Health check executed before normal operation.
- Results recorded internally.
- Fault conditions reported to WAN.

### Note

Motor health parameters will be defined later.

Potential parameters may include:

- Current consumption
- Supply voltage
- Runtime duration
- Overload condition
- Dry run detection
- Temperature
- Relay status

---

## 9 Fault Management

### FR-MASTER-012: Slave Communication Failure Handling

The Master Node shall detect communication failures with Slave Nodes.

### Acceptance Criteria

- Missing acknowledgements shall be detected.
- Retry mechanism shall be activated.
- Communication timeout shall be logged.
- Failure status shall be reported to WAN.

---

### FR-MASTER-013: WAN Communication Failure Handling

The Master Node shall detect WAN connectivity issues.

### Acceptance Criteria

- Connectivity failures shall be logged.
- Pending reports shall be buffered.
- Reports shall be forwarded when WAN connectivity is restored.

---

## 10 Future Enhancements

### FR-MASTER-014: OTA Update Management

The Master Node should support future OTA update distribution to Slave Nodes.

---

### FR-MASTER-015: Event Logging

The Master Node should maintain an event history for:

- Command reception
- Slave responses
- Motor operations
- Communication failures
- Battery alerts

---

### FR-MASTER-016: Centralized System Diagnostics

The Master Node should provide complete diagnostic information for:

- WAN Interface
- LoRa Interface
- Slaves
- Motor Controller
- Power Subsystem