import { IoClose } from "react-icons/io5";
import useService from "../../lib/useService";
import { useState } from "react";
import { TfiText } from "react-icons/tfi";
import { GoNumber } from "react-icons/go";
import { MdDataArray, MdDataObject, MdOutlineToggleOn } from "react-icons/md";
import { FaTrash } from "react-icons/fa";
import { showToast } from "../../lib/toast";

const FieldTypes = [
  {
    id: 1,
    name: "string",
    description: "Text, image URL, markdown, etc.",
    icon: <TfiText />,
  },
  {
    id: 2,
    name: "number",
    description: "Integer, float, etc.",
    icon: <GoNumber />,
  },
  {
    id: 3,
    name: "boolean",
    description: "true or false",
    icon: <MdOutlineToggleOn />,
  },
  {
    id: 4,
    name: "array",
    description: "List of items",
    icon: <MdDataArray />,
  },
  {
    id: 5,
    name: "object",
    description: "Nested object",
    icon: <MdDataObject />,
  },
];

export default function CollectionCreateModal({ close }) {
  const [collectionName, setCollectionName] = useState("");
  const [schema, setSchema] = useState({});
  const [newField, setNewField] = useState("");
  const [newFieldType, setNewFieldType] = useState("string");
  const [embeddingKeys, setEmbeddingKeys] = useState([]);
  const { collectionService } = useService();

  const createCollection = async () => {
    try {
      await collectionService.createCollection({
        collection_name: collectionName,
        collection_schema: {
          type: "object",
          properties: schema,
        },
        enable_auto_embedding: embeddingKeys.length > 0,
        embedding_keys: embeddingKeys,
      });
      showToast("Collection created successfully");
      close();
    } catch (error) {
      console.error(error);
    }
  };

  return (
    <div className="modal-main">
      <h3>Create new collection</h3>
      <label>Collection Name</label>
      <input
        className="input"
        type="text"
        value={collectionName}
        onChange={(e) => setCollectionName(e.target.value)}
      />
      <label>Schema</label>
      <div className="schema-form">
        <input
          className="input"
          type="text"
          value={newField}
          onChange={(e) => setNewField(e.target.value)}
          placeholder="Field name"
        />
        <select
          className="select"
          value={newFieldType}
          onChange={(e) => setNewFieldType(e.target.value)}
        >
          {FieldTypes.map((field) => (
            <option key={field.id} value={field.name}>
              {field.name}
            </option>
          ))}
        </select>
        <button
          className="button"
          onClick={() => {
            setSchema({
              ...schema,
              [newField]: {
                type: newFieldType,
              },
            });
            setNewField("");
            setNewFieldType("string");
          }}
          disabled={!newField}
        >
          Add Field
        </button>
      </div>
      <div className="schema-list stretch">
        {Object.keys(schema).map((key) => {
          const type = schema[key].type;
          return (
            <div key={key} className="schema-item">
              {FieldTypes.find((field) => field.name === type).icon}
              <h4>{key}</h4>
              <p>{type}</p>
              <p>
                auto embedding:{" "}
                <span>{embeddingKeys.includes(key) ? "true" : "false"}</span>
              </p>
              {type === "string" && (
                <button
                  className="button"
                  onClick={() => {
                    setEmbeddingKeys(
                      embeddingKeys.includes(key)
                        ? embeddingKeys.filter((k) => k !== key)
                        : [...embeddingKeys, key]
                    );
                  }}
                >
                  {embeddingKeys.includes(key) ? "Disable" : "Enable"} Auto
                  Embedding
                </button>
              )}
              <button
                className="button negative icon"
                onClick={() => {
                  const newSchema = { ...schema };
                  delete newSchema[key];
                  setSchema(newSchema);
                }}
              >
                <FaTrash />
              </button>
            </div>
          );
        })}
      </div>
      <button
        className="button"
        disabled={Object.keys(schema).length === 0 || !collectionName}
        onClick={createCollection}
      >
        Create Collection
      </button>
      <div className="close-button" onClick={close}>
        <IoClose />
      </div>
    </div>
  );
}
