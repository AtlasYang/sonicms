import { AnimatePresence, motion } from "framer-motion";
import { useEffect, useState } from "react";
import ReactDOM from "react-dom";

const useModal = () => {
  const [isModalOpen, setIsModalOpen] = useState(false);
  const [modalContainer, setModalContainer] = useState(null);

  useEffect(() => {
    const modalRoot = document.getElementById("modal-root");
    setModalContainer(modalRoot);
  }, []);

  const openModal = () => {
    setIsModalOpen(true);
  };
  const closeModal = () => {
    setIsModalOpen(false);
  };

  const renderModal = (children) => {
    if (!modalContainer) return null;
    return ReactDOM.createPortal(
      <AnimatePresence>
        {isModalOpen && (
          <motion.div
            key={modalContainer.id}
            style={{
              position: "fixed",
              top: "0",
              left: "0",
              width: "100%",
              height: "100%",
              backgroundColor: "rgba(255, 255, 255, 0.3)",
              backdropFilter: "blur(5px)",
              display: "flex",
              justifyContent: "center",
              alignItems: "center",
              transition: "all 0.2s",
              zIndex: "1000",
            }}
            initial={{ opacity: 0 }}
            animate={{ opacity: 1 }}
            exit={{ opacity: 0 }}
            transition={{ duration: 0.2 }}
          >
            {children}
          </motion.div>
        )}
      </AnimatePresence>,
      modalContainer
    );
  };

  return {
    isModalOpen,
    openModal,
    closeModal,
    renderModal,
  };
};

export default useModal;
