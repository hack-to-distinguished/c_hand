import axios from "axios";


type SocketRef = React.RefObject<WebSocket | null>;
type MessageObject = {
  sender_id?: string;
  recipient_id?: string;
  send_time?: string; // ISO string preferred
  message?: string;
  [key: string]: any;
};

export const handleMessage = ({
  socket,
  setMessages,
  event,
  activeTab
}: {
  socket: SocketRef;
  setMessages: React.Dispatch<React.SetStateAction<MessageObject[]>>;
  event: MessageEvent;
  activeTab: string | null;
}) => {
  if (!socket?.current) return;

  let parsed: MessageObject | null = null;
  try {
    parsed = JSON.parse(event.data);
  } catch (err) {
    // Fallback: treat the incoming data as a raw message string
    parsed = { message: event.data };
  }

  // Ensure we append a consistent object shape
  const now = new Date();
  const month = now.toLocaleString('en-UK', { month: 'short' });
  const day = now.getDate().toString().padStart(2, '0');
  const year = now.getFullYear();
  const time = now.toTimeString().split(' ')[0];

  const formattedNow = `${month} ${day} ${time} ${year}`;

  const messageObj: MessageObject = {
    ...parsed,
    sender_id: parsed?.sender_id ?? "unknown",
    recipient_id:
      typeof parsed?.recipient_id === "string" && parsed.recipient_id
        ? parsed.recipient_id
        : "all",
    send_time: parsed?.send_time ?? formattedNow,
    message: parsed?.message ?? "",
  };
  console.log("Adding message object to tab: ", messageObj, activeTab);
  // TODO: if the active tab is also the tab where the message is destined
  // add it to the message object, if not skip
  // if (messageObj.recipient_id && messageObj.recipient_id != activeTab) {
  //   return;
  // }
  setMessages((prevMessages) => [...prevMessages, messageObj]);
  return;
};

export const sendMessage = async ({
  socket,
  message,
}: {
  socket: SocketRef;
  message: MessageObject;
}) => {
  if (!socket?.current) {
    console.log("Cannot send message. No socket available.");
    return;
  }
  console.log("Message object to send:", message);

  if (socket.current.readyState === 1) {
    try {
      const payload = JSON.stringify(message);
      socket.current.send(payload);
      console.log("Sent JSON message:", payload);

    } catch (err) {
      console.log("Error stringifying/sending message:", err);
    }
  } else {
    console.log("Cannot send message. WebSocket is not open.");
  }
};

export const getAllMessages = async () => {
  try {
    const response = await axios.get("http://localhost:8081/messages");
    const msgData = response.data;
    console.log("message data", msgData);

    let msgString = "";
    for (let i = 0; i < msgData.length; i++) {
      const c = msgData[i];
      if (msgData[i - 1] === " " || msgData[i - 1] === ":" || msgData[i - 1] === "{" || msgData[i + 1] === "}" || msgData[i + 1] === ":" || msgData[i + 1] === ","){
        msgString += c === "'" ? '"' : c;
      } else {
        msgString += c;
      }
    }
    console.log("reconstructed string:", msgString);
    return JSON.parse(msgString);

  } catch (error) {
    console.log("Error getting all messages:", error);
  }
};

export const getConversationMessages = async (senderId: string) => {
  try {
    // use this url or pass some extra information in the get request to specify the sender id
    const response = await axios.get(`http://localhost:8081/messages/${senderId}`);
    // const response = await axios.get("http://localhost:8081/messages");
    const msgData = response.data;
    console.log("message data", msgData);

    let msgString = "";
    for (let i = 0; i < msgData.length; i++) {
      const c = msgData[i];
      if (msgData[i - 1] === " " || msgData[i - 1] === ":" || msgData[i - 1] === "{" || msgData[i + 1] === "}" || msgData[i + 1] === ":" || msgData[i + 1] === ","){
        msgString += c === "'" ? '"' : c;
      } else {
        msgString += c;
      }
    }
    console.log("reconstructed string:", msgString);
    return JSON.parse(msgString);

  } catch (error) {
    console.log(`Error ${error} getting messages for sender: ${senderId}`);
    return [];
  }
}
