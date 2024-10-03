import { motion } from "framer-motion";

export default function PageWrapper({ children }) {
  return (
    <motion.div
      initial={{ opacity: 0, x: 50 }}
      animate={{ opacity: 1, x: 0 }}
      exit={{ opacity: 0, x: 50 }}
      transition={{
        type: "spring",
        stiffness: 400,
        damping: 40,
      }}
      className="screen-main"
    >
      {children}
    </motion.div>
  );
}
