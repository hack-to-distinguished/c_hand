import { useState, useEffect } from "react";
import { useWebSocket } from "../services/handleNetwork.tsx";
import NetworkStatus from "../components/networkStatus.tsx";
import MessageBox from "../components/messageBox.tsx";
import MessageFeed from "../components/messageFeed.tsx";
import UserSelection from "../components/userSelection.tsx";
import { generateId } from "../services/handleUser.tsx";
import { setInLocalStorage } from "../services/handleBrowser.tsx";
import "./messageDisplay.css";


interface SavedMessages {
  send_time?: string;
  sender_id?: string;
  message?: string;
  [key: string]: any;
}

function MessageDisplay() {
  const serverUrl = "ws://127.0.0.1:8081";
  const { socket, connectionStatus } = useWebSocket(serverUrl);

  const [userName, setUserName] = useState<string>("");
  const [messagesObject, setMessagesObject] = useState<SavedMessages[]>([]);

  useEffect(() => {
    // initialize username from localStorage (or generate one)
    if (userName) return;

    let username = localStorage.getItem("username");
    if (!username) {
      username = generateId(5);
      setInLocalStorage("username", username);
    }
    // TODO: make initial register post request in this use effect


    setUserName(username || "");
  }, [userName]);


  return (
    <>
      <UserSelection userName={userName} setUserName={setUserName} />
      <NetworkStatus connectionStatus={connectionStatus} />
      <MessageFeed socket={socket} connectionStatus={connectionStatus} messagesObject={messagesObject} setMessagesObject={setMessagesObject} />
      <MessageBox socket={socket} connectionStatus={connectionStatus} userName={userName} setMessagesObject={setMessagesObject} />
    </>
  );
}

export default MessageDisplay;
