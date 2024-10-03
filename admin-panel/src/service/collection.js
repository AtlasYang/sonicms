export class CollectionService {
  constructor(instance) {
    this.instance = instance;
  }

  async getCollections() {
    const res = await this.instance.get("/collection");

    return res.data;
  }

  async getCollection(collectionId) {
    const res = await this.instance.get(`/collection/${collectionId}`);

    return res.data;
  }

  async createCollection(collection) {
    const res = await this.instance.post("/collection", collection);

    return res.data;
  }

  async updateCollection(collectionId, collection) {
    const res = await this.instance.put(
      `/collection/${collectionId}`,
      collection
    );

    return res.data;
  }

  async deleteCollection(collectionId) {
    const res = await this.instance.post(`/collection/delete/${collectionId}`);

    return res.data;
  }
}
