## Rust based peer-to-peer transport and sync layer for arbitrary application data.

### Goal:
Move and synchronise arbitrary data (messages, state updates, blobs, request response traffic) directly between peers with minimal server dependence.

Existing "serverless" data products are narrow tools: file transfer, sync, or browser only channels. Here I'm creating a reusable Rust node that handles identity, peer discrover, encrypted transport, replication, and application-facing APIs for structured data exchange

This is the foundation for direct sync, app-to-app messaging, distributed job dispatch, and offline-tolerant state replication

### Objectives:
- [ ] Peer identity and authenticated connections
- [ ] Secure transport
- [ ] Request-reponse messaging for arbitrary payload
- [ ] Websocket connection for sync
- [ ] Pub-Sub event broadcast
- [ ] CLI and library for integrating the node into applications

##### Secondary objectives:
- [ ] Consumer file-sharing app




## Architecture outline:
1. Identity layer
- Each node gets a persistent peer identity and sings or authenticates sessions so that peer know who they are connected to. The identity is the vector for trust, permissions, peer addressing, ...

2. Transport layer
- Rust-libp2p

3. Session and protocol layer
- Request-response RPC style traffic
- Streaming payload transfer (likely Websockets)
- Replicated state updates (likely Websockets)

4. Data model layer
- Application-agnostic. The nodes move envelopes with typed metadata, version information, payload bytes, optional conflict resolution hints.

5. Replication layer
- Support for multiple replication modes so the system is useful across products:
-    - Direct send, append-only event log, keyed record sync, blob transfer.

6. Connectivity layer
- Difficult networks inclusion: Direct connections, NAT traversal, browser-facing (WebRTC/DataChannel)

7. Developer API
- Rust library API and CLI tool
- Let apps embed a node, advertise protocols, send messags, subscribe to data changes, inspect peer state without foricing the app to understand the full network


## MVP
1. Run rust nodes locally and over a network
2. Establish authenticated peer connections
3. Send arbitrary payload between peers
4. Replicate small shared collections (notes, records, events)
5. Handle reconnect and catch-up after temporary disconnection
6. Expose all of this through CLI and a small API surface



## Milestones
1. Node skeleton
- [ ] Create crate layout
- [ ] Define peer identity model
- [ ] Basic libp2p swarm
- [ ] Connect two peers and verify liveness

2. Generic message
- [ ] Add request-response messaging.
- [ ] Define and envelope format with message type, correlation ID, payload
- [ ] Add serialisation and validation boundaries

3. Replication
- [ ] Simple append-only log or document store
- [ ] Implement sync and reconnect
- [ ] Add versioning/confict policy

4. Hard networks
- [ ] Measure direct-connect success
- [ ] Add relay or alternate connectivity path where required


## Decisions to make:
- Peer discovery model? Manual address, LAN, rendez-vous, hybrid?
- Trust model? Open network, allowlist, invite codes, signed capabilities?
- Replication primitive? Log, CRDT, document sync, request-response only?
- Payload framing? Binary first or JSON first?
- Storage engine? Memory first, then embedded persistence?
- Failure policy? Eventual consistency, explicit retries, durable outbound queue?






## Tech
- libp2p
- WebRTC
