export const SampleCollectionTypes = [
  {
    id: 1,
    name: "Article",
    schema: [
      { name: "title", type: "string" },
      { name: "thumbnail", type: "string" },
      { name: "content", type: "string" },
      { name: "author", type: "string" },
      { name: "published_at", type: "string" },
      { name: "category", type: "string" },
      { name: "tags", type: "array", items: { type: "string" } },
    ],
  },
  {
    id: 2,
    name: "Product",
    schema: [
      { name: "name", type: "string" },
      { name: "price", type: "number" },
      { name: "description", type: "string" },
      { name: "category", type: "string" },
      { name: "tags", type: "array", items: { type: "string" } },
    ],
  },
  {
    id: 3,
    name: "User",
    schema: [
      { name: "username", type: "string" },
      { name: "email", type: "string" },
      { name: "password", type: "string" },
      { name: "profile_picture", type: "string" },
      { name: "role", type: "string" },
      { name: "created_at", type: "string" },
    ],
  },
];
