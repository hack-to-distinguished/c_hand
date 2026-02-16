import axios from "axios"; 

export const getAllMessages = async () => {
  try {
    const response = await axios.get("http://127.0.0.1:8081/messages");
    console.log("Response:", response);
    return response;

  } catch (error) {
    console.log("Error getting response:", error);
  }
};
