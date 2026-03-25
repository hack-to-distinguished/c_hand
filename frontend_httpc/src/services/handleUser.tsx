import axios from "axios"; 

export const generateId = (length) => {
  
  let result = '';
  const characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
  const charactersLength = characters.length;
  
  for (let i = 0; i < length; i++ ) {
    result += characters.charAt(Math.floor(Math.random() * charactersLength));
  }
  
  return result;
}


export const registerUser = async (username) => {
  console.log("Registering user with username:", username);
  
  try {
    const response = await axios.post("http://127.0.0.1:8081/register", {
      username: username, 
      headers: { 'Content-Type': 'application/json' }
    });
    console.log("Reponse:", response);
  } catch (error) {
    console.log("Service registering user error", error);
  }
}