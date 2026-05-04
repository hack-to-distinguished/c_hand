import { useState, useEffect, useRef } from "react";
import { getAllMessages, getMessagesFromSenderId } from "../services/handleMessages.tsx";
import { handleMessage as handleIncomingMessage } from "../services/handleMessages.tsx";
import "./messageFeed.css";

interface MessageObject {
  send_time?: string;
  sender_id?: string;
  message?: string;
  [key: string]: any;
}

interface ConnectedUser {
  username: string;
  lastActiveTime: string;
}

interface MessageFeedProps {
  socket: React.RefObject<WebSocket | null>;
  connectionStatus: string;
  messagesObject: MessageObject[];
  setMessagesObject: React.Dispatch<React.SetStateAction<MessageObject[]>>;
  connectedUsersList: ConnectedUser[];
  activeTab: string|null;
  setActiveTab: React.Dispatch<React.SetStateAction<string|null>>;
  tabMessages: MessageObject;
  setTabMessages: React.Dispatch<React.SetStateAction<MessageObject[]>>;
}

const MessageFeed = ({
  socket, messagesObject, setMessagesObject, connectedUsersList,
  activeTab, setActiveTab, tabMessages, setTabMessages
}: MessageFeedProps) => {
  const [completedInitialRequest, setCompletedInitialRequest] = useState<boolean>(false);

  // null -> "All" tab is active - string -> sender_id of the active tab
  const [isLoadingTab, setIsLoadingTab] = useState<boolean>(false);

  const listRef = useRef<HTMLUListElement | null>(null);

  const initialGetMessagesReq = async () => {
    try {
      console.log("Attempting to get all messages on tab:", activeTab);
      const messages = await getAllMessages();
      if (messages) {
        console.log("All messages:", messages);

        setMessagesObject(messages);
        setTabMessages(messages)
        setCompletedInitialRequest(true);
      }
    } catch (error) {
      console.log("Unable to get messages:", error);
    }
  };

  useEffect(() => {
    if (completedInitialRequest) return;
    console.log("request not done", completedInitialRequest);
    initialGetMessagesReq();
  }, [completedInitialRequest]);

  // Keep the "all" tab in sync with live WebSocket messages
  useEffect(() => {
    if (activeTab === null) {
      setTabMessages(messagesObject);
    }
  }, [messagesObject]);

  // Scroll to the bottom whenever the list changes
  useEffect(() => {
    listRef.current?.lastElementChild?.scrollIntoView();
    // TODO: Fix the scroll to view
  }, [messagesObject]);

  useEffect(() => {
    if (!socket.current) return;

    const onMessage = (event: MessageEvent) => {
      // Reuse the shared handler shape: parse and append
      handleIncomingMessage({ socket, setMessages: setMessagesObject, event, activeTab });
      // TODO: change the handleIncomingMessage to only display/get the messages directed
      // to all users
    };

    socket.current.onmessage = onMessage;

    return () => {
      if (socket.current) {
        socket.current.onmessage = null;
      }
    };
  }, [socket.current]);

  const handleTabClick = async (senderId: string | null) => {
    setActiveTab(senderId);

    if (senderId === null) {
      setTabMessages(messagesObject);
      return;
    }

    setIsLoadingTab(true);
    try {
      const result = await getMessagesFromSenderId(senderId);
      setTabMessages(result ?? []);

      // TODO: Add the same handleIncomingMessage from the socket
      // here (or elsewhere (most likely elsewhere)) to handle messages
      // specific to the user.


    } catch (error) {
      console.log("Error fetching tab messages:", error);
      setTabMessages([]);
    } finally {
      setIsLoadingTab(false);
    }
  };

  return (
    <div className="messages-display">

      <div className="messages-tabs">
          <button
            onClick={() => handleTabClick(null)}
            className={activeTab === null ? 'active' : ''}
          >
            All
          </button>

        {connectedUsersList.map((user) => (
          <button
            key={user.username}
            onClick={() => handleTabClick(user.username)}
            className={activeTab === user.username ? "active" : ""}
          >
            {user.username}
          </button>
          ))}

        {/* Message list */}

      </div>
      <div className="messages-tabs-content">
        {isLoadingTab ? (
          <p className="empty-message">Loading messages...</p>
        ) : (
          <ul className="messages-list" ref={listRef}>
            {tabMessages.length === 0 ? (
              <li className="empty-message">No messages yet</li>
            ) : (
              tabMessages.map((message: MessageObject, i: number) => (
                <li key={i}>
                  <span className="message-metadata">
                    {message.send_time} - From { message.sender_id }:
                  </span>{" "}
                  {message.message}
                </li>
              ))
            )}
          </ul>
        )}
      </div>


    </div>
  );
};

export default MessageFeed;
