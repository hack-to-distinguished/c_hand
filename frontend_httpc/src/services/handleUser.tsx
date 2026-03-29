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
    // because cors isn't set up in the C backend, we need to make the request to the same TLD as the frontend
    const response = await axios.post("http://localhost:8081/register", {
      username: username, 
      // headers: { 'Content-Type': 'application/json' }
      headers: { 'Content-Type': 'text/html' }
    });
    console.log("Registering user response:", response);
  } catch (error) {
    console.log("Service registering user error", error);
  }
}