import { useState, useEffect } from "react";
import { getAllMessages } from "../services/getMessages.tsx";
import "./messageFeed.css";

interface MessageBoxProps {
  socket: React.RefObject<WebSocket | null>;
  connectionStatus: string;
}

const MessageFeed = ({ socket }: MessageBoxProps) => {
  const [messages, setMessages] = useState<string[]>([]);
  const [completedInitialRequest, setCompletedInitialRequest] = useState<boolean>(false);

  useEffect(() => {
    const initialGetMessagesReq = async () => {
      if (!completedInitialRequest) {
        try {
          console.log("Attempting to get all messages")
          const message = await getAllMessages();
          console.log("message data:", message?.data);
          const messagesString = message?.data;
          let newString = "";
          for (let i = 0; i < messagesString.length; i++) {
            const c = messagesString[i];
            newString += c === "'" ? '"' : c;
          }
          console.log("message data type:", JSON.parse(newString));
        
          const savedMessages = JSON.parse(newString);
          // setMessages((prevMessages) => [...prevMessages, savedMessages.values()])

          setCompletedInitialRequest(true);
        } catch (error) {
          console.log("Unable to get messages:", error);
        }
      }
    };
    initialGetMessagesReq();
  }, [completedInitialRequest]);

  useEffect(() => {
    if (!socket.current) return;

    const handleMessage = (event: MessageEvent) => {
      const receivedMessage = event.data;
      console.log("Event", socket.current);
      console.log("Message from server:", receivedMessage);
      setMessages((prevMessages) => [...prevMessages, receivedMessage]);
    };

    socket.current.onmessage = handleMessage;

    return () => {
      if (socket.current) {
        socket.current.onmessage = null;
      }
    };
  }, [socket.current]);

  return (
    <div className="messages-display">
      <div className="message-header">HTTP_C Chat</div>
      <ul className="messages-list">
        {messages.length === 0 ? (
          <li className="empty-message">No messages yet.</li>
        ) : (
          messages.map((msg, i) => (
            <li key={i}>
              <span style={{ color: "#003366", fontWeight: "bold" }}>
                {new Date().toLocaleTimeString()} - Message {i + 1}:
              </span>{" "}
              {msg}
            </li>
          ))
        )}
      </ul>
    </div>
  );
};

export default MessageFeed;
