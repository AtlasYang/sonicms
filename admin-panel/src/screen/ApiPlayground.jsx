import { useEffect, useState } from "react";
import PageWrapper from "../component/PageWrapper";
import useService from "../lib/useService";
import { showToast } from "../lib/toast";

export default function ApiPlayground() {
  const [collections, setCollections] = useState([]);
  const [selectedCollection, setSelectedCollection] = useState(null);
  const [allEntries, setAllEntries] = useState([]);
  const [searchResults, setSearchResults] = useState([]);
  const [entryId, setEntryId] = useState(null);
  const [searchLimit, setSearchLimit] = useState(10);
  const { collectionService, entryService } = useService();

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
      if (entries.length === 0) {
        showToast("No entries found", "error");
      }
      setAllEntries(entries);
    } catch (error) {
      console.error(error);
    }
  };

  const searchEntries = async () => {
    try {
      const entries = await entryService.searchEntries(
        selectedCollection.collection_name,
        entryId,
        searchLimit
      );
      if (entries.length === 0) {
        showToast("No entries found", "error");
      }
      setSearchResults(entries);
    } catch (error) {
      console.error(error);
    }
  };

  useEffect(() => {
    fetchCollections();
  }, []);

  return (
    <PageWrapper>
      <h1>API Playground</h1>
      <h5>Choose a collection to start</h5>
      <select
        className="select"
        onChange={(e) => {
          const collectionName = e.target.value;
          const collection = collections.find(
            (collection) => collection.collection_name === collectionName
          );
          setSelectedCollection(collection);
          setAllEntries([]);
          setSearchResults([]);
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
      {selectedCollection && (
        <>
          <label>Read all entries</label>
          <div className="screen-api-box">
            <p>
              <span className="clickable" onClick={fetchEntries}>
                GET
              </span>{" "}
              /api/
              {selectedCollection.collection_name}
            </p>
            <p>Get all entries in the collection</p>
            {allEntries.length > 0 && (
              <div className="code-box">
                <pre>{JSON.stringify(allEntries, null, 2)}</pre>
              </div>
            )}
          </div>
          {selectedCollection.enable_auto_embedding && (
            <>
              <label>Vector Search</label>
              <div className="screen-api-box">
                <p>
                  <span className="clickable" onClick={searchEntries}>
                    GET
                  </span>{" "}
                  /api-search/
                  {selectedCollection.collection_name}/
                  <input
                    className="input-inline"
                    type="text"
                    placeholder="Entry ID"
                    onChange={(e) => setEntryId(e.target.value)}
                  />
                  ?limit=
                  <input
                    className="input-inline small"
                    type="number"
                    value={searchLimit}
                    onChange={(e) => setSearchLimit(e.target.value)}
                  />
                </p>
                <p>Search for similar entries based on the provided entry ID</p>
                {searchResults.length > 0 && (
                  <div className="code-box">
                    <pre>{JSON.stringify(searchResults, null, 2)}</pre>
                  </div>
                )}
              </div>
            </>
          )}
        </>
      )}
    </PageWrapper>
  );
}
