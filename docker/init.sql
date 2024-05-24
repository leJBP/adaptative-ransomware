--
-- PostgreSQL database cluster dump
--

-- Started on 2024-05-23 22:07:40

SET default_transaction_read_only = off;

SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;

--
-- Roles
--

CREATE ROLE postgres;
ALTER ROLE postgres WITH SUPERUSER INHERIT CREATEROLE CREATEDB LOGIN REPLICATION BYPASSRLS PASSWORD 'SCRAM-SHA-256$4096:QG2F1iDNvhKBR87xap1n7Q==$UojTjvJ2p//Tak/XKaJ/uXgHjwuKmdpdO9R6a/jFm1k=:5XTNJTcBo2r7NuzmCnkyi6j43f/mtUJcICQ6qP8hFIU=';

--
-- User Configurations
--








--
-- Databases
--

--
-- Database "template1" dump
--

\connect template1

--
-- PostgreSQL database dump
--

-- Dumped from database version 16.2 (Debian 16.2-1.pgdg120+2)
-- Dumped by pg_dump version 16.2

-- Started on 2024-05-23 22:07:40

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

-- Completed on 2024-05-23 22:07:40

--
-- PostgreSQL database dump complete
--

--
-- Database "postgres" dump
--

\connect postgres

--
-- PostgreSQL database dump
--

-- Dumped from database version 16.2 (Debian 16.2-1.pgdg120+2)
-- Dumped by pg_dump version 16.2

-- Started on 2024-05-23 22:07:40

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

-- Completed on 2024-05-23 22:07:40

--
-- PostgreSQL database dump complete
--

--
-- Database "projet_crypto" dump
--

--
-- PostgreSQL database dump
--

-- Dumped from database version 16.2 (Debian 16.2-1.pgdg120+2)
-- Dumped by pg_dump version 16.2

-- Started on 2024-05-23 22:07:41

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 3360 (class 1262 OID 16384)
-- Name: projet_crypto; Type: DATABASE; Schema: -; Owner: postgres
--

CREATE DATABASE projet_crypto WITH TEMPLATE = template0 ENCODING = 'UTF8' LOCALE_PROVIDER = libc LOCALE = 'en_US.utf8';


ALTER DATABASE projet_crypto OWNER TO postgres;

\connect projet_crypto

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 215 (class 1259 OID 16385)
-- Name: asymetricKeys; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."asymetricKeys" (
    identifier character varying(255) NOT NULL,
    "publicKey" text NOT NULL,
    "privateKey" text NOT NULL
);


ALTER TABLE public."asymetricKeys" OWNER TO postgres;

--
-- TOC entry 216 (class 1259 OID 16398)
-- Name: symetricKeys; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."symetricKeys" (
    identifier character varying NOT NULL,
    key text NOT NULL
);


ALTER TABLE public."symetricKeys" OWNER TO postgres;

--
-- TOC entry 3353 (class 0 OID 16385)
-- Dependencies: 215
-- Data for Name: asymetricKeys; Type: TABLE DATA; Schema: public; Owner: postgres
--



--
-- TOC entry 3354 (class 0 OID 16398)
-- Dependencies: 216
-- Data for Name: symetricKeys; Type: TABLE DATA; Schema: public; Owner: postgres
--



--
-- TOC entry 3207 (class 2606 OID 16391)
-- Name: asymetricKeys keys_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."asymetricKeys"
    ADD CONSTRAINT keys_pkey PRIMARY KEY (identifier);


--
-- TOC entry 3209 (class 2606 OID 16404)
-- Name: symetricKeys symetricKeys_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."symetricKeys"
    ADD CONSTRAINT "symetricKeys_pkey" PRIMARY KEY (identifier);


-- Completed on 2024-05-23 22:07:41

--
-- PostgreSQL database dump complete
--

-- Completed on 2024-05-23 22:07:41

--
-- PostgreSQL database cluster dump complete
--

