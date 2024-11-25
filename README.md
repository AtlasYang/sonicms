<p align="center">
  <a>
   <img src="https://storage.luidium.lighterlinks.io/media/sonicms-logo.png" alt="logo" width="420px">
  </a>
</p>

<h3 align="center">Cloud-friendly, high-performance headless CMS</h3>
<p align="center">Fast, scalable, and reliable content delivery system built on top of C++</p>

<p align="center">
  <a>
   <img src="https://storage.luidium.lighterlinks.io/media/sonicms-demo-1.gif" alt="demo" width="720px">
  </a>
</p>

## Key Features

- **Cloud-friendly Architecture**: Designed to easily deploy on cloud environments with scalability and flexibility in mind.
- **High Performance**: Built with C++ for efficient processing and rapid response times.
- **Object Storage**: MinIO is used to manage and serve large media files efficiently, providing S3-compatible storage.
- **Scalable Database**: Powered by ScyllaDB, offering low-latency and high-throughput database operations, ensuring optimal performance even under heavy loads.
- **Vector Database**: Integrated with Qdrant for efficient vector-based similarity search on embeddings.
- **Fast Text Embeddings**: Leverages C++ ONNX runtime and C++ bindings for a Rust-based Tokenizer to extract embeddings at high speed, with similarity search endpoints available out of the box.
- **Dockerized Setup**: Deploy all components including the CMS API, Admin Panel, Object Storage (MinIO), and ScyllaDB with a single `docker compose` command.
- **Admin Panel**: Manage content easily through a React-powered admin interface.

## Tech Stack

- **Backend**: C++ for high-performance API handling.
- **Database**: [ScyllaDB](https://www.scylladb.com/) for low-latency, high-throughput database operations.
- **Vector Search**: [Qdrant](https://qdrant.tech/) for similarity search using embeddings.
- **Object Storage**: [MinIO](https://min.io/) for scalable object storage.
- **Frontend**: Admin panel built with [React](https://react.dev)

## Getting Started

### Prerequisites

Ensure that you have the following installed:

- **Docker**: [Install Docker](https://docs.docker.com/get-docker/)
- **Docker Compose**: [Install Docker Compose](https://docs.docker.com/compose/install/)

### Installation

1. **Clone the Repository**:

   ```sh
   git clone https://github.com/AtlasYang/sonicms.git
   cd sonicms
   ```

2. **Create the `.env` File**:

   You can use the provided `.env.template` as a base by modifying the placeholders according to your setup (e.g., database credentials, API ports, MinIO access keys). After making the necessary changes, rename it to `.env`.

   For example:

   ```sh
   cp .env.template .env
   ```

   Then, update the values in `.env`:

   ```env
   ADMIN_PANEL_PORT=3000
   CMS_API_PORT=8080
   MINIO_URL=http://yourdomain.com/storage/
   MINIO_ACCESS_KEY=your-access-key
   MINIO_SECRET_KEY=your-secret-key
   ...
   ```

3. **Run Docker Compose**:

   Once the `.env` file is ready, navigate to the project root directory and start the CMS by running:

   ```bash
   docker compose up --build
   ```

   This will build and start all the necessary services, including:

   - **SonicMS API Server**: The core CMS API server.
   - **Admin Panel**: The React-powered admin interface.
   - **ScyllaDB**: The high-performance database.
   - **MinIO**: The object storage service for managing large media files.
   - **Qdrant**: The vector search engine.

4. **Claim MinIO Access Key and Secret Key**:

   Open your browser and navigate to `http://localhost:[MINIO_WEB_ADMIN_PORT]` to access the MinIO admin console.
   In the admin console, go to **Access Keys** and click **Create Access Key**.

   Once generated, update the `.env` file with the new credentials:

   - `STORAGE_ACCESS_KEY` = _[your new access key]_
   - `STORAGE_SECRET_KEY` = _[your new secret key]_

5. **Access the Application**:

   You can customize exposed ports in `.env` file.

   - **Admin Panel**: Navigate to `http://localhost:[ADMIN_PANEL_PORT]` to access the admin panel.
   - **CMS API**: The API will be available at `http://localhost:[CMS_API_PORT]`.

### **Deployment**

This section will guide you through deploying SonicMS using Nginx as a reverse proxy and setting up domain registration, SSL certificates, and exposing MinIO storage.

#### 1. Nginx Reverse Proxy Setup

To deploy SonicMS behind an Nginx reverse proxy, follow these steps:

1.  **Install Nginx**:
    On your server, install Nginx using the package manager appropriate for your operating system.

    ```sh
    sudo apt update
    sudo apt install nginx
    ```

2.  **Configure Nginx**:
    Create a new Nginx configuration file for SonicMS under `/etc/nginx/sites-available/sonicms.conf`:

    ```sh
    sudo nano /etc/nginx/sites-available/sonicms.conf
    ```

    Add the following configuration to reverse proxy requests to the CMS API, Admin Panel, and MinIO storage:

    ```nginx
      server {
         listen 80;
         server_name yourdomain.com;  # Replace with your actual domain name

         # Redirect all HTTP traffic to HTTPS
         return 301 https://$host$request_uri;
      }

      server {
         listen 443 ssl;
         server_name yourdomain.com; # Replace with your actual domain name

         # SSL Configuration
         ssl_certificate /etc/letsencrypt/live/yourdomain.com/fullchain.pem;  # Generated by Certbot
         ssl_certificate_key /etc/letsencrypt/live/yourdomain.com/privkey.pem;  # Generated by Certbot

         # Proxy for CMS API
         location / {
            proxy_pass http://127.0.0.1:[CMS_API_PORT];  # Replace with the CMS API port from .env
            proxy_set_header Host $host;
            proxy_set_header X-Real-IP $remote_addr;
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
            proxy_set_header X-Forwarded-Proto $scheme;
         }

         # Proxy for Admin Panel
         location /admin/ {
            proxy_pass http://127.0.0.1:[ADMIN_PANEL_PORT];  # Replace with the Admin Panel port from .env
            proxy_set_header Host $host;
            proxy_set_header X-Real-IP $remote_addr;
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
            proxy_set_header X-Forwarded-Proto $scheme;
         }

         # Proxy for MinIO Storage
         upstream sonicms_storage {
            server 127.0.0.1:[MINIO_PORT];  # Replace with the Minio port from .env
         }

         location /storage/ {
            proxy_set_header Host $http_host;
            proxy_set_header X-Real-IP $remote_addr;
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
            proxy_set_header X-Forwarded-Proto $scheme;
            proxy_connect_timeout 300;
            # Default is HTTP/1, keepalive is only enabled in HTTP/1.1
            proxy_http_version 1.1;
            proxy_set_header Connection "";
            chunked_transfer_encoding off;
            proxy_pass http://sonicms_storage;
         }
      }
    ```

3.  **Enable the Nginx Configuration**:
    After configuring Nginx, enable the configuration by creating a symlink to the `sites-enabled` directory and restart Nginx:

    ```sh
    sudo ln -s /etc/nginx/sites-available/sonicms.conf /etc/nginx/sites-enabled/
    sudo systemctl restart nginx
    ```

#### 2. Domain Registration

To link your server with a domain:

1.  **Register a domain** with any registrar (e.g. Cloudflare, Namecheap, GoDaddy, etc.).
2.  **Update DNS settings** to point to your server's IP address.

Once the DNS changes propagate, the domain will point to your server where SonicMS is running.

#### 3. SSL Setup with Let's Encrypt

To secure your domain with SSL, use Let's Encrypt to install and manage SSL certificates:

1.  **Install Certbot**:

    ```sh
    sudo apt install certbot python3-certbot-nginx
    ```

2.  **Generate and Install SSL Certificates**:

    ```sh
    sudo certbot --nginx -d yourdomain.com
    ```

Certbot will automatically update your Nginx configuration to serve SonicMS securely via HTTPS. Make sure to stop nginx before using certbot.

#### 4. Exposing MinIO Storage

MinIO handles object storage, and you’ll need to expose it via Nginx for media files. Assuming you have already set MinIO up in the `docker-compose.yaml`, make sure MinIO is bound to the correct port (9000 by default).

- **MinIO URL Configuration**:
  You can access MinIO via the URL `http://yourdomain.com/storage/` or set up a subdomain like `storage.yourdomain.com` for easier access.

- **Customize the `.env` File** to include the MinIO URL:
  Add this line to your `.env` file:

```env
MINIO_URL=http://yourdomain.com/storage/
```

#### 5. Port Binding

Ensure you expose and customize the ports in the `.env` file as needed for SonicMS components:

```env
ADMIN_PANEL_PORT=3000
CMS_API_PORT=8080
MINIO_PORT=9000
```

Make sure these ports are available and not in conflict with other services on your server. For production, running everything behind Nginx with ports 80 (HTTP) and 443 (HTTPS) is recommended.

## Project Structure

```sh
sonicms/
├── admin-panel/                # React Admin Panel
├── cms-api/                    # CMS API Backend
├── database/                   # ScyllaDB Setup
├── compose.yaml                # Docker Compose File
├── .env                        # Environment Variables
├── .env.template               # Template for Environment Variables
├── LICENSE                     # MIT LICENSE File
└── README.md                   # Project Documentation
```

## Contributing

We welcome contributions to **SonicMS**! Please feel free to submit pull requests, create issues, or suggest improvements.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
