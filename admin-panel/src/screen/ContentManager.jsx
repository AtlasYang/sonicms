import { useEffect, useState } from "react";
import PageWrapper from "../component/PageWrapper";
import useService from "../lib/useService";
import useModal from "../lib/useModal";
import { unixTimestampToDateTimeString } from "../util/timestamp";
import EntryModal from "../component/modal/EntryModal";
import EntryCreateModal from "../component/modal/EntryCreateModal";

export default function ContentManager() {
  const [collections, setCollections] = useState([]);
  const [selectedCollection, setSelectedCollection] = useState(null);
  const [allEntries, setAllEntries] = useState([]);
  const [selectedEntry, setSelectedEntry] = useState(null);
  const { collectionService, entryService } = useService();
  const {
    openModal: openEntryModal,
    renderModal: renderEntryModal,
    closeModal: closeEntryModal,
  } = useModal();
  const {
    openModal: openEntryCreateModal,
    renderModal: renderEntryCreateModal,
    closeModal: closeEntryCreateModal,
  } = useModal();

  const fetchCollections = async () => {
    try {
      const collections = await collectionService.getCollections();
      setCollections(collections);
    } catch (error) {
      console.error(error);
    }
  };

  const fetchEntries = async () => {
    try {
      const entries = await entryService.getEntries(
        selectedCollection.collection_name
      );
      setAllEntries(entries);
    } catch (error) {
      console.error(error);
    }
  };

  useEffect(() => {
    fetchCollections();
  }, []);

  useEffect(() => {
    if (selectedCollection) {
      fetchEntries();
    }
  }, [selectedCollection]);

  return (
    <PageWrapper>
      <h1>Content Manager</h1>
      <h5>Choose a collection to start</h5>
      <select
        className="select"
        onChange={(e) => {
          const collectionName = e.target.value;
          const collection = collections.find(
            (collection) => collection.collection_name === collectionName
          );
          setSelectedCollection(collection);
        }}
      >
        <option>Select a collection</option>
        {collections.map((collection) => {
          return (
            <option key={collection.collection_id}>
              {collection.collection_name}
            </option>
          );
        })}
      </select>
      <button
        className="button"
        onClick={openEntryCreateModal}
        disabled={!selectedCollection}
      >
        Create Entry
      </button>
      {selectedCollection && (
        <div className="entry-list">
          {allEntries.map((entry) => {
            return (
              <div
                key={entry.entry_id}
                className="entry-item"
                onClick={() => {
                  setSelectedEntry(entry);
                  openEntryModal();
                }}
              >
                <h3>{entry.entry_id}</h3>
                <p>
                  created at{" "}
                  <span>{unixTimestampToDateTimeString(entry.created_at)}</span>
                </p>
                <p>
                  last updated at{" "}
                  <span>{unixTimestampToDateTimeString(entry.updated_at)}</span>
                </p>
              </div>
            );
          })}
          {allEntries.length === 0 && (
            <p>No entries found in this collection</p>
          )}
        </div>
      )}
      {selectedEntry &&
        renderEntryModal(
          <EntryModal
            collectionName={selectedCollection.collection_name}
            entry={selectedEntry}
            close={() => {
              closeEntryModal();
              fetchEntries();
            }}
          />
        )}
      {selectedCollection &&
        renderEntryCreateModal(
          <EntryCreateModal
            collection={selectedCollection}
            close={() => {
              closeEntryCreateModal();
              fetchEntries();
            }}
          />
        )}
    </PageWrapper>
  );
}
