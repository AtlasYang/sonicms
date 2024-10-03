import { GoNumber } from "react-icons/go";
import { IoClose } from "react-icons/io5";
import { MdDataArray, MdDataObject, MdOutlineToggleOn } from "react-icons/md";
import { TfiText } from "react-icons/tfi";
import useService from "../../lib/useService";
import { showToast } from "../../lib/toast";
import { useState } from "react";
import { unixTimestampToDateTimeString } from "../../util/timestamp";

export default function EntryModal({ collectionName, entry, close }) {
  const { entryService } = useService();
  const [updatedData, setUpdatedData] = useState(Object.assign({}, entry.data));

  const deleteEntry = async () => {
    try {
      await entryService.deleteEntry(collectionName, entry.entry_id);
      showToast("Entry deleted successfully", "success");
      close();
    } catch (error) {
      console.error(error);
    }
  };

  const updateEntry = async () => {
    try {
      await entryService.updateEntry(
        collectionName,
        entry.entry_id,
        updatedData
      );
      showToast("Entry updated successfully", "success");
      close();
    } catch (error) {
      console.error(error);
    }
  };

  return (
    <div className="modal-main">
      <h4>
        {collectionName} - {entry.entry_id}
      </h4>
      <p>
        Last updated:{" "}
        <span>{unixTimestampToDateTimeString(entry.updated_at)}</span>
      </p>
      <label>Data</label>
      <div className="schema-list stretch">
        {entry.data &&
          Object.keys(entry.data).map((key) => {
            const type = typeof entry.data[key];
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
                    type="text"
                    value={updatedData[key]}
                    onChange={(e) => {
                      setUpdatedData({ ...updatedData, [key]: e.target.value });
                    }}
                  />
                )}
                {type === "number" && (
                  <input
                    className="input"
                    type="number"
                    value={updatedData[key]}
                    onChange={(e) => {
                      setUpdatedData({
                        ...updatedData,
                        [key]: Number(e.target.value),
                      });
                    }}
                  />
                )}
                {type === "boolean" && (
                  <select
                    className="select"
                    value={updatedData[key]}
                    onChange={(e) => {
                      setUpdatedData({
                        ...updatedData,
                        [key]: e.target.value === "true",
                      });
                    }}
                  >
                    <option value="true">true</option>
                    <option value="false">false</option>
                  </select>
                )}
              </div>
            );
          })}
      </div>
      <div className="button-group">
        <button className="button negative" onClick={deleteEntry}>
          <span>Delete</span>
        </button>
        <button className="button" onClick={updateEntry}>
          <span>Update</span>
        </button>
      </div>
      <div className="close-button" onClick={close}>
        <IoClose />
      </div>
    </div>
  );
}
