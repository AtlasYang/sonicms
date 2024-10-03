import toast from "react-hot-toast";

export const showToast = (message, type = "success") => {
  toast[type](message, {
    duration: 1400,
    style: {
      borderRadius: "5px",
      background: "#FFD735",
      color: "#FFFFFF",
    },
    iconTheme: {
      primary: "#FFFFFF",
      secondary: "#FFD735",
    },
  });
};
