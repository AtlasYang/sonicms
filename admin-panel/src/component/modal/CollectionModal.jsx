import { GoNumber } from "react-icons/go";
import { IoClose } from "react-icons/io5";
import { MdDataArray, MdDataObject, MdOutlineToggleOn } from "react-icons/md";
import { TfiText } from "react-icons/tfi";

export default function CollectionModal({ collection, close }) {
  return (
    <div className="modal-main">
      <h3>{collection.collection_name}</h3>
      <label>Schema</label>
      <div className="schema-list">
        {collection.collection_schema.properties &&
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
                <p>{type}</p>
                <p>
                  auto embedding:{" "}
                  <span>
                    {collection.embedding_keys?.includes(key)
                      ? "true"
                      : "false"}
                  </span>
                </p>
              </div>
            );
          })}
      </div>
      <div className="close-button" onClick={close}>
        <IoClose />
      </div>
    </div>
  );
}
