import { GoNumber } from "react-icons/go";
import { IoClose } from "react-icons/io5";
import { MdDataArray, MdDataObject, MdOutlineToggleOn } from "react-icons/md";
import { TfiText } from "react-icons/tfi";
import useService from "../../lib/useService";
import { showToast } from "../../lib/toast";
import { useState } from "react";

export default function EntryCreateModal({ collection, close }) {
  const { entryService } = useService();
  const [entryData, setEntryData] = useState({});

  const createEntry = async () => {
    try {
      await entryService.createEntry(collection.collection_name, entryData);
      showToast("Entry created successfully", "success");
      close();
    } catch (error) {
      console.error(error);
    }
  };

  return (
    <div className="modal-main">
      <h4>{collection.collection_name}</h4>
      <label>Data</label>
      <div className="schema-list stretch">
        {collection.collection_schema &&
          Object.keys(collection.collection_schema.properties).map((key) => {
            const type = collection.collection_schema.properties[key].type;
            return (
              <div key={key} className="schema-item">
                {
                  {
                    string: <TfiText />,
                    number: <GoNumber />,
                    boolean: <MdOutlineToggleOn />,
                    array: <MdDataArray />,
                    object: <MdDataObject />,
                  }[type]
                }
                <h4>{key}</h4>
                {type === "string" && (
                  <textarea
                    className="textarea"
                    rows={3}
                    type="text"
                    value={entryData[key] || ""}
                    onChange={(e) => {
                      setEntryData({ ...entryData, [key]: e.target.value });
                    }}
                  />
                )}
                {(type === "number" || type === "integer") && (
                  <input
                    className="input"
                    type="number"
                    value={entryData[key] || 0}
                    onChange={(e) => {
                      setEntryData({
                        ...entryData,
                        [key]: Number(e.target.value),
                      });
                    }}
                  />
                )}
                {type === "boolean" && (
                  <select
                    className="select"
                    value={entryData[key] || ""}
                    onChange={(e) => {
                      setEntryData({
                        ...entryData,
                        [key]: e.target.value === "true",
                      });
                    }}
                  >
                    <option>Select</option>
                    <option value={true}>true</option>
                    <option value={false}>false</option>
                  </select>
                )}
              </div>
            );
          })}
      </div>
      <div className="button-group">
        <button
          className="button"
          onClick={createEntry}
          disabled={Object.keys(collection.collection_schema.properties).some(
            (key) => entryData[key] === undefined
          )}
        >
          <span>Create</span>
        </button>
      </div>
      <div className="close-button" onClick={close}>
        <IoClose />
      </div>
    </div>
  );
}
