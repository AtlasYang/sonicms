FROM node:20.11-alpine AS build

WORKDIR /app

COPY ./package.json ./
COPY ./package-lock.json ./

RUN npm install

COPY . .

ARG VITE_API_URL

RUN echo "VITE_API_URL=$VITE_API_URL" > .env

RUN npm run build

FROM node:20.11-alpine

WORKDIR /app

RUN npm install -g serve

COPY --from=build /app/dist ./dist

EXPOSE 3000

CMD ["serve", "-s", "dist", "-l", "3000"]
