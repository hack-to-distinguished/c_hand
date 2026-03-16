import { useState, useEffect } from "react";
import { useWebSocket } from "../services/network.tsx";
import NetworkStatus from "../components/networkStatus.tsx";
import MessageBox from "../components/messageBox.tsx";
import MessageFeed from "../components/messageFeed.tsx";
import UserSelection from "../components/userSelection.tsx";
import { generateId } from "../services/generateRandom";
import { setInLocalStorage } from "../services/browserStorage";
import "./messageDisplay.css";

function MessageDisplay() {
  const serverUrl = "ws://127.0.0.1:8081";
  const { socket, connectionStatus } = useWebSocket(serverUrl);

  const [userName, setUserName] = useState<string>("");

  useEffect(() => {
    // initialize username from localStorage (or generate one)
    if (userName) return;

    let username = localStorage.getItem("username");
    if (!username) {
      username = generateId(5);
      setInLocalStorage("username", username);
    }
    setUserName(username || "");
  }, [userName]);

  return (
    <>
      <UserSelection userName={userName} setUserName={setUserName} />
      <NetworkStatus connectionStatus={connectionStatus} />
      <MessageFeed socket={socket} connectionStatus={connectionStatus} />
      <MessageBox socket={socket} connectionStatus={connectionStatus} userName={userName} />
    </>
  );
}

export default MessageDisplay;