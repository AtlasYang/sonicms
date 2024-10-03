import axios from "axios";
import { CollectionService } from "../service/collection";
import { EntryService } from "../service/entry";

export default function useService() {
  const instance = axios.create({
    baseURL: "http://localhost:9233",
    headers: {
      "Content-Type": "application/json",
    },
  });

  return {
    collectionService: new CollectionService(instance),
    entryService: new EntryService(instance),
  };
}
