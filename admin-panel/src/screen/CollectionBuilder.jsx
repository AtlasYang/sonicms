import { useEffect, useState } from "react";
import PageWrapper from "../component/PageWrapper";
import { unixTimestampToDateString } from "../util/timestamp";
import useService from "../lib/useService";
import useModal from "../lib/useModal";
import CollectionModal from "../component/modal/CollectionModal";
import CollectionCreateModal from "../component/modal/CollectionCreateModal";
import { BiCollection } from "react-icons/bi";
import { showToast } from "../lib/toast";

export default function CollectionBuilder() {
  const [collections, setCollections] = useState([]);
  const [selectedCollection, setSelectedCollection] = useState(null);
  const { collectionService } = useService();
  const {
    openModal: openCollectionModal,
    renderModal: renderCollectionModal,
    closeModal: closeCollectionModal,
  } = useModal();
  const {
    openModal: openCreateModal,
    renderModal: renderCreateModal,
    closeModal: closeCreateModal,
  } = useModal();

  const fetchCollections = async () => {
    try {
      const collections = await collectionService.getCollections();
      setCollections(collections);
    } catch (error) {
      console.error(error);
    }
  };

  useEffect(() => {
    fetchCollections();
  }, []);

  return (
    <PageWrapper>
      <h1>Collection Builder</h1>
      <div className="screen-box clickable" onClick={openCreateModal}>
        <BiCollection size={32} />
        <div>
          <h3>Build collections with custom fields.</h3>
          <p>Define the schema of your collection and start adding data.</p>
        </div>
      </div>
      <label>Collections</label>
      <div className="collection-list">
        {collections.map((collection) => {
          return (
            <div
              key={collection.collection_id}
              className="collection-item"
              onClick={() => {
                setSelectedCollection(collection);
                openCollectionModal();
              }}
            >
              <h3>{collection.collection_name}</h3>
              <p>
                created at{" "}
                <span>{unixTimestampToDateString(collection.created_at)}</span>
              </p>
              <p>
                auto embedding:{" "}
                <span>
                  {collection.enable_auto_embedding ? "enabled" : "disabled"}
                </span>
              </p>
              <button
                className="button negative"
                onClick={async (e) => {
                  e.stopPropagation();
                  try {
                    await collectionService.deleteCollection(
                      collection.collection_id
                    );
                    showToast("Collection deleted successfully");
                    fetchCollections();
                  } catch (error) {
                    console.error(error);
                  }
                }}
              >
                <span>Delete</span>
              </button>
            </div>
          );
        })}
      </div>
      {selectedCollection &&
        renderCollectionModal(
          <CollectionModal
            collection={selectedCollection}
            close={closeCollectionModal}
          />
        )}
      {renderCreateModal(
        <CollectionCreateModal
          close={() => {
            closeCreateModal();
            fetchCollections();
          }}
        />
      )}
    </PageWrapper>
  );
}
