---CREATE DATABASE carrent;

DROP TABLE IF EXISTS rents;
DROP TABLE IF EXISTS cars;
DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS destinations;
DROP TABLE IF EXISTS car_types;

CREATE TABLE public.users (
    id bigserial,
    name character varying(255),
    surname character varying(255),
    address character varying(255),
    credit_card character varying(255),
    driving_license character varying(255),
    createdAt timestamp with time zone NOT NULL default now(),
    updatedAt timestamp with time zone NOT NULL default now(),
    CONSTRAINT users_pkey PRIMARY KEY (id)
);

ALTER TABLE public.users
    ADD CONSTRAINT field_descs_unique_dom_id UNIQUE (name, surname);

CREATE TABLE public.car_types (
    "name" character varying(25),
    price integer,
    speed integer,
    max_persons smallint,
	service_km smallint,
	service_price smallint,
    createdAt timestamp with time zone NOT NULL default now(),
    updatedAt timestamp with time zone NOT NULL default now(),
    CONSTRAINT car_types_pkey PRIMARY KEY ("name")
);

CREATE TABLE public.cars (
    id bigserial,
    "name" character varying(255),
    brand character varying(255),
    tail_number character varying(255),
	"class" character varying(25),
	is_free boolean default true,
    createdAt timestamp with time zone NOT NULL default now(),
    updatedAt timestamp with time zone NOT NULL default now(),
    CONSTRAINT cars_pkey PRIMARY KEY (id),
	CONSTRAINT "cars_fkey" FOREIGN KEY ("class") REFERENCES "public"."car_types" ("name") ON UPDATE CASCADE ON DELETE CASCADE
);
ALTER TABLE public.cars
    ADD CONSTRAINT cars_unique_dom_id UNIQUE (name);


CREATE TABLE public.destinations (
    id bigserial,
    "name" character varying(255),
    before bigint,
    after bigint,
    hops integer,
    createdAt timestamp with time zone NOT NULL default now(),
    updatedAt timestamp with time zone NOT NULL default now(),
    CONSTRAINT destinations_pkey PRIMARY KEY (id)
);

CREATE TABLE public.rents (
    id bigserial,
	name character varying(255),
    userid bigint,
	car bigint,
	fromloc bigint,
	toloc bigint,
	passengers smallint,
    date_from timestamp with time zone NOT NULL default now(),
    date_to timestamp with time zone NOT NULL default now(),

    createdAt timestamp with time zone NOT NULL default now(),
    updatedAt timestamp with time zone NOT NULL default now(),
    CONSTRAINT rents_pkey PRIMARY KEY (id),
	CONSTRAINT "rents_user_fkey" FOREIGN KEY (userid) REFERENCES "public"."users" (id) ON UPDATE CASCADE ON DELETE CASCADE,
	CONSTRAINT "rents_car_fkey" FOREIGN KEY (car) REFERENCES "public"."cars" (id) ON UPDATE CASCADE ON DELETE CASCADE,
	CONSTRAINT "rents_from_fkey" FOREIGN KEY (fromloc) REFERENCES "public"."destinations" (id) ON UPDATE CASCADE ON DELETE CASCADE,
	CONSTRAINT "rents_to_fkey" FOREIGN KEY (toloc) REFERENCES "public"."destinations" (id) ON UPDATE CASCADE ON DELETE CASCADE
);

INSERT INTO "car_types" ("name", "price", "speed", "max_persons", "service_km", "service_price") VALUES ('ECO', 1, 15, 2, 1500, 300);
INSERT INTO "car_types" ("name", "price", "speed", "max_persons", "service_km", "service_price") VALUES ('MID-CLASS', 2, 25, 4, 1500, 300);
INSERT INTO "car_types" ("name", "price", "speed", "max_persons", "service_km", "service_price") VALUES ('DELUXE', 5, 50, 7, 1500, 300);

INSERT INTO "destinations" ("id", "name", "before", "after", "hops") VALUES (1, 'Inner Circle', 0, 2, 1);
INSERT INTO "destinations" ("id", "name", "before", "after", "hops") VALUES (2, 'Outer Circle', 3, 0, 1);
INSERT INTO "destinations" ("id", "name", "before", "after", "hops") VALUES (3, 'Middle Circle', 1, 3, 1);

INSERT INTO "users" ("id", "name", "surname", "address", "credit_card", "driving_license") VALUES (1, 'Fabio', 'Cosimelli', 'via Verdi', '111.111.111', 'AA1111');

INSERT INTO "cars" ("id", "name", "brand", "tail_number", "class", "is_free") VALUES (1, 'Nuvolina', 'Dacia', 'XX111XX', 'ECO', 'true');

