import axios from "axios";
import { CollectionService } from "../service/collection";
import { EntryService } from "../service/entry";

export default function useService() {
  const instance = axios.create({
    baseURL: import.meta.env.VITE_API_URL,
    headers: {
      "Content-Type": "application/json",
    },
  });

  return {
    collectionService: new CollectionService(instance),
    entryService: new EntryService(instance),
  };
}
