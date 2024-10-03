export class EntryService {
  constructor(instance) {
    this.instance = instance;
  }

  async getEntries(collectionName) {
    const res = await this.instance.get(`/api/${collectionName}`);

    return res.data;
  }

  async getEntry(collectionName, entryId) {
    const res = await this.instance.get(`/api/${collectionName}/${entryId}`);

    return res.data;
  }

  async createEntry(collectionName, entry) {
    const res = await this.instance.post(`/api/${collectionName}`, {
      data: entry,
    });

    return res.data;
  }

  async updateEntry(collectionName, entryId, entry) {
    const res = await this.instance.post(
      `/api/update/${collectionName}/${entryId}`,
      {
        data: entry,
      }
    );

    return res.data;
  }
  async deleteEntry(collectionName, entryId) {
    const res = await this.instance.post(
      `/api/delete/${collectionName}/${entryId}`
    );

    return res.data;
  }

  async searchEntries(collectionName, entryId, limit) {
    const res = await this.instance.get(
      `/api-search/${collectionName}/${entryId}?limit=${limit}`
    );

    return res.data;
  }
}
