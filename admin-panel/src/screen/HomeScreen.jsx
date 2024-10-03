import { FaGithub } from "react-icons/fa";
import { PiFeatherFill } from "react-icons/pi";
import { MdSpaceDashboard } from "react-icons/md";
import { RiTestTubeFill } from "react-icons/ri";
import PageWrapper from "../component/PageWrapper";

export default function HomeScreen() {
  return (
    <PageWrapper>
      <h1>Welcome to SoniCMS Admin!</h1>
      <label>Check out our latest updates on GitHub:</label>
      <div className="screen-box">
        <FaGithub />
        <a
          href="https://github.com/AtlasYang/sonicms"
          target="_blank"
          rel="noreferrer"
        >
          GitHub Repository
        </a>
      </div>
      <h2>Quick Tour</h2>
      <div className="screen-box">
        <PiFeatherFill size={32} />
        <div>
          <h3>Collection Builder</h3>
          <p>Build collections with custom fields.</p>
        </div>
      </div>
      <div className="screen-box">
        <MdSpaceDashboard size={32} />
        <div>
          <h3>Content Manager</h3>
          <p>
            Manage content for your collections. Add, edit, and delete content.
          </p>
        </div>
      </div>
      <div className="screen-box">
        <RiTestTubeFill size={32} />
        <div>
          <h3>API Playground</h3>
          <p>Test the API endpoints for your collections in real-time.</p>
        </div>
      </div>
    </PageWrapper>
  );
}
