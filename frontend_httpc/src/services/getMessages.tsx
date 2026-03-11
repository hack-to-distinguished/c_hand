import axios from "axios"; 

export const getAllMessages = async () => {
  try {
    const response = await axios.get("http://127.0.0.1:8081/messages");
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
    console.log("Error getting response:", error);
  }
};
