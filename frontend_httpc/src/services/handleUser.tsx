import axios from "axios";

export const generateId = (length: number) => {

  let result = '';
  const characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
  const charactersLength = characters.length;

  for (let i = 0; i < length; i++ ) {
    result += characters.charAt(Math.floor(Math.random() * charactersLength));
  }

  return result;
}


export const registerUser = async (username: string) => {
  console.log("Registering user with username:", username);

  try {
    const response = await axios.post(
      "http://127.0.0.1:8081/register",
      { username: username },
      { headers: { 'Content-Type': 'application/json' } }
    );
    console.log("Registering user response:", response);
  } catch (error) {
    console.log("Service registering user error", error);
  }
}

export const getAllUsers = async (username: string) => {
  try {
    console.log(`Getting list of all users other than ${username}`)
    const response = await axios.get("http://127.0.0.1:8081/users");
    const userData = response.data;
    console.log("users data", userData);

    if (!userData || typeof userData !== "object") {
      return [];
    }
    delete userData[username];

    // Transform object into list
    const usersArray = Object.entries(userData).map(([username, info]) => {
      const anyInfo: any = info as any;
      let epochSec = 0;

      if (anyInfo.last_message_send_time && anyInfo.last_message_send_time > 0) {
        epochSec = anyInfo.last_message_send_time;
      } else if (anyInfo.connected_at && anyInfo.connected_at > 0) {
        epochSec = anyInfo.connected_at;
      }

      const lastActiveTime = epochSec && epochSec > 0
        ? new Date(epochSec * 1000).toLocaleString() : "";

      return {
        username, lastActiveTime
      };
    });

    return usersArray;

  } catch (error) {
    console.log("Error getting list of users:", error);
    return [];
  }
};
