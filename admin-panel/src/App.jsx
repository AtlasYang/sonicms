import "./App.css";
import "./style/screen.css";
import "./style/modal.css";
import { useState } from "react";
import { FaHome } from "react-icons/fa";
import { PiFeatherFill } from "react-icons/pi";
import { MdSpaceDashboard } from "react-icons/md";
import { RiTestTubeFill } from "react-icons/ri";
import HomeScreen from "./screen/HomeScreen";
import CollectionBuilder from "./screen/CollectionBuilder";
import { AnimatePresence, motion } from "framer-motion";
import { Toaster } from "react-hot-toast";
import ContentManager from "./screen/ContentManager";
import ApiPlayground from "./screen/ApiPlayground";

function TagP({ children, id }) {
  return (
    <motion.p
      id={`tag-p-${id}`}
      initial={{ opacity: 0, x: -20 }}
      animate={{ opacity: 1, x: 0 }}
      exit={{ opacity: 0, x: -20 }}
      transition={{ duration: 0.5, delay: 0.2 }}
    >
      {children}
    </motion.p>
  );
}

const MenuItems = [
  {
    id: 0,
    icon: <FaHome />,
    text: "Home",
  },
  {
    id: 1,
    icon: <PiFeatherFill />,
    text: "Collection Builder",
  },
  {
    id: 2,
    icon: <MdSpaceDashboard />,
    text: "Content Manager",
  },
  {
    id: 3,
    icon: <RiTestTubeFill />,
    text: "API Playground",
  },
];

function App() {
  const [selected, setSelected] = useState(0);

  const handleClickMenu = (index) => {
    setSelected(index);
  };

  return (
    <div className="main">
      <div className="sidebar">
        <img
          className="sidebar-img-big"
          src="logo-text.svg"
          alt="logo"
          width="100%"
        />
        <img
          className="sidebar-img-small"
          src="logo.svg"
          alt="logo"
          width={40}
        />
        <div className="menu">
          {MenuItems.map((item) => {
            return (
              <div
                key={item.id}
                onClick={() => {
                  handleClickMenu(item.id);
                }}
                className={`menu-item ${selected === item.id && "selected"}`}
              >
                {item.icon}
                <TagP id={item.id}>{item.text}</TagP>
              </div>
            );
          })}
        </div>
      </div>
      <div className="content">
        <AnimatePresence>
          {selected === 0 && <HomeScreen />}
          {selected === 1 && <CollectionBuilder />}
          {selected === 2 && <ContentManager />}
          {selected === 3 && <ApiPlayground />}
        </AnimatePresence>
      </div>
      <Toaster position="top-right" />
    </div>
  );
}

export default App;
