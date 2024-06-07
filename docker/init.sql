DROP TABLE IF EXISTS public."asymetricKeys";
DROP TABLE IF EXISTS public."symetricKeys";

-- Création des tables
CREATE TABLE public."asymetricKeys"
(
    identifier character varying NOT NULL,
    "publicKey" text NOT NULL,
    "privateKey" text NOT NULL,
    algorithm character varying COLLATE pg_catalog."default",
    PRIMARY KEY (identifier)
);
ALTER TABLE public."asymetricKeys" OWNER to postgres;

CREATE TABLE public."symetricKeys"
(
    identifier character varying NOT NULL,
    key text NOT NULL,
    algorithm character varying COLLATE pg_catalog."default",
    infos text NOT NULL,
    PRIMARY KEY (identifier)
);
ALTER TABLE public."symetricKeys" OWNER to postgres;
