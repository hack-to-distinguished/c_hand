# What type of data is being shared
# How is it copied
# What guarantees the copy process provides


# 4 Nodes:

### 1. Direct send
- One peer sends a payload to another peer

### 2. Append-only event log
- Every change is written in order. Peers can replay the log to reconstruct state

### 3. Keyed record sync
- Each record has a key, peers exchange inserts, updates, and deletes for those keys. The system doesn't necessarily care about the full history, only the latest or merged version of each key

### 4. Blob transfer
- For large opaque payloads: files, attachments, media, model checkpoints, or binary archives.
- Needs chunking, integrity checks, resume support, and de-duplication
- Different from the other 3 because the system is moving content, not updating small structured fields


## What the replication layer owns:
- The data model (events, records, blobs)
- Versioning
- Conflict strategy
- Sync protocol
- Recovery
