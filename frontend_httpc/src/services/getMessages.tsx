import axios from "axios"; 

export const getAllMessages = async () => {
  try {
    const response = await axios.get("http://127.0.0.1:8081/messages");
    const msgData = response.data;
    // console.log("message data", msgData);

    let msgString = "";
    for (let i = 0; i < msgData.length; i++) {
      const c = msgData[i];
      msgString += c === "'" ? '"' : c;
    }
    return JSON.parse(msgString);

  } catch (error) {
    console.log("Error getting response:", error);
  }
};
