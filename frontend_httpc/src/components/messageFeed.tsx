import { useState, useEffect, useRef } from "react";
import { getAllMessages } from "../services/handleMessages.tsx";
import { handleMessage as handleIncomingMessage } from "../services/handleMessages.tsx";
import "./messageFeed.css";

interface MessageBoxProps {
  socket: React.RefObject<WebSocket | null>;
  connectionStatus: string;
  send_time?: string;
  sender_id?: string;
  message?: string;
  [key: string]: any;
}

const MessageFeed = ({ socket, messagesObject, setMessagesObject }: MessageBoxProps ) => {
  const [completedInitialRequest, setCompletedInitialRequest] = useState<boolean>(false);
  const listRef = useRef<HTMLUListElement | null>(null);

  const initialGetMessagesReq = async () => {
    try {
      console.log("Attempting to get all messages");
      const messages = await getAllMessages();
      if (messages) {
        console.log("All messages:", messages);

        // Assume messages is already an array of structured objects
        setMessagesObject(messages);
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

  useEffect(() => {
    listRef.current?.lastElementChild?.scrollIntoView();
  }, [messagesObject]);

  useEffect(() => {
    if (!socket.current) return;

    const onMessage = (event: MessageEvent) => {
      // Reuse the shared handler shape: parse and append
      handleIncomingMessage({ socket, setMessages: setMessagesObject, event });
    };

    socket.current.onmessage = onMessage;

    return () => {
      if (socket.current) {
        socket.current.onmessage = null;
      }
    };
  }, [socket.current]);

  return (
    <div className="messages-display">
      <div className="message-header">Corpo Chat</div>
      <ul className="messages-list" ref={listRef}>
        {messagesObject.length === 0 ? (
          <li className="empty-message">No messages yet</li>
        ) : (
          messagesObject.map((message, i) => (
            <li key={i}>
              <span style= {{ color: "#003366", fontWeight: "bold" }}>
                {message.send_time} - From { message.sender_id }: 
              </span>{" "}
              {message.message}
            </li>
          ))
        )}
      </ul>
    </div>
  );
};

export default MessageFeed;
