
export const getCookie = (name) => {
  const value = `; ${document.cookie}`;
  const parts = value.split(`; ${name}=`);
  if (parts.length === 2 ) return parts.pop().split(';').shift();
  return null
};



export const setInLocalStorage = async(key, value) => {
  localStorage.setItem(key, value);
}