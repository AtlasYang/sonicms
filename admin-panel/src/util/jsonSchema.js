import Ajv from "ajv";

export function generateJsonSchema(fields) {
  const properties = {};
  fields.forEach((field) => {
    if (field.type === "object") {
      properties[field.name] = generateJsonSchema(field.fields);
      return;
    }

    if (field.type === "array") {
      properties[field.name] = {
        type: "array",
        items: generateJsonSchema(field.items),
      };
      return;
    }

    properties[field.name] = {
      type: field.type,
    };
  });

  return {
    type: "object",
    properties,
  };
}

export function validateJsonSchema(jsonSchema, data) {
  const ajv = new Ajv();
  const validate = ajv.compile(jsonSchema);
  return validate(data);
}
