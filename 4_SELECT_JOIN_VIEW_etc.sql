USE	[CSOFT_PHONEBOOK]
GO
/**
Извежда информация за местоживеене, код на град, ID на абонат, ЕГН, и имена на абонати като събира информацията от таблица PERSONS и CITIES
**/
SELECT 
	[PERSONS].[ID], 
	[FIRST_NAME], 
	[LAST_NAME], 
	[UCN], 
	[ADDRESS], 
	[NAME] AS CITY,
	[CITY_ID] 
FROM [PERSONS]
INNER JOIN [CITIES] 
	ON CITIES.ID = PERSONS.CITY_ID

/**
Извежда първо и фамилно име на абонат и град, в който живее
**/
SELECT 
	(PERSONS.FIRST_NAME + ' ' + PERSONS.LAST_NAME) AS NAME, 
	CITIES.NAME AS CITY  
FROM [PERSONS]
RIGHT JOIN [CITIES] 
	ON PERSONS.CITY_ID = CITIES.ID
GO

/**
Изглед за извеждане на имена и номер на абонат
**/
CREATE OR ALTER VIEW VI_PHONE_NUMBERS_AND_OWNERS_VARNA_REGION
AS
	SELECT 
		(PERSONS.FIRST_NAME + ' ' + PERSONS.LAST_NAME) AS NAME, 
		PHONE_NUMBERS.PHONE
	FROM [PHONE_NUMBERS]
	INNER JOIN [PERSONS]
		ON PHONE_NUMBERS.PERSON_ID = PERSONS.ID
	INNER JOIN [CITIES]
		ON PERSONS.CITY_ID = CITIES.ID
	WHERE CITIES.REGION = 'VARNA'
GO

/**
Използване на изгледа за извеждане на имена и номер на абонат
**/
SELECT * FROM VI_PHONE_NUMBERS_AND_OWNERS_VARNA_REGION
GO

/**
Изглед за извеждане на телефони и техния тип
**/
CREATE OR ALTER VIEW VI_PHONE_NUMBERS_AND_PHONE_TYPES 
AS
	SELECT 
		PHONE_TYPES.TYPE,
		PHONE_NUMBERS.PHONE AS PHONE_NUMBER
	FROM [PHONE_TYPES]
	LEFT JOIN [PHONE_NUMBERS]
		ON PHONE_NUMBERS.PHONE_TYPE_ID = PHONE_TYPES.ID
	LEFT JOIN [PERSONS]
		ON PHONE_NUMBERS.PERSON_ID = PERSONS.ID
	LEFT JOIN [CITIES]
		ON PERSONS.CITY_ID = CITIES.ID
GO


/**
Използване на изгледа за извеждане на телефони и техния тип
**/
SELECT * FROM VI_PHONE_NUMBERS_AND_PHONE_TYPES
GO


/**
Изглед за извеждане на брой популацията в даден град
**/
CREATE OR ALTER VIEW VI_POPULATION_BY_CITIES
AS
	SELECT
		CITIES.NAME,
		COUNT(*) AS [POPULATION]
	FROM [PERSONS]
	INNER JOIN [CITIES] 
		ON PERSONS.CITY_ID = CITIES.ID
	GROUP BY
		CITIES.NAME
	--HAVING COUNT(*) > 1
GO
/**
Използване на изгледа за извеждане на брой популацията в даден град
**/
SELECT * FROM VI_POPULATION_BY_CITIES

/**
Извежда номер на абонат (ID), телефонен номер и ID на типа на телефон
**/
SELECT PERSON_ID, PHONE, ID FROM PHONE_NUMBERS

/**
Извежда всички записи за градове
**/
SELECT * FROM CITIES