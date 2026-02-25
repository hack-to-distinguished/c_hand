import { useState, useEffect} from "react";
import { getAllMessages } from "../services/getMessages.tsx";
import "./messageFeed.css";

interface MessageBoxProps {
  socket: React.RefObject<WebSocket | null>;
  connectionStatus: string;
}

interface SavedMessages {
  message?: string;
  sendTime?: string;
  senderId?: string;
}

const MessageFeed = ({ socket }: MessageBoxProps) => {
  const [messages, setMessages] = useState<string[]>([]);
  const [messagesObject, setMessagesObject] = useState<[SavedMessages]>([{
    message: "default", sendTime: "default", senderId: "default"
  }]);
  const [completedInitialRequest, setCompletedInitialRequest] = useState<boolean>(false);

  const initialGetMessagesReq = async () => {
    try {
      console.log("Attempting to get all messages")
      const messages = await getAllMessages();
      if (messages) {
        console.log("All messages:", messages);

        const userMessages = messages.map((userObj) => userObj.message);

        const messageCount = Object.keys(messages).length;
        console.log(`There are ${messageCount} messages`);
        for (let i = 0; i < messageCount; i++){
          setMessagesObject(messagesObject => ({
            ...messagesObject, // (spread operator) needed to make sure we don't overwrite all the values our object
            message: messages[i].message, sendTime: messages[i].send_time, senderId: messages[i].sender_id
          }));

        }

        setMessages(userMessages)
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

  useEffect(() => { // Only used for logging whenever messageObject changes
    console.log("message object", messagesObject);
  }, [messagesObject]);

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
