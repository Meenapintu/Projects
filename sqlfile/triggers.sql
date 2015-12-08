CREATE OR REPLACE FUNCTION update_returnH_trigg_function()
  RETURNS trigger AS
$BODY$
DECLARE

BEGIN
    IF (TG_TABLE_NAME = 'returnhistoryinfo') THEN
        RAISE NOTICE 'TRIGER called on claim %', TG_TABLE_NAME;
    END IF;
    IF (TG_OP = 'INSERT') THEN
    UPDATE booksinfo set available = 't' where bookid=NEW.bookid;
    UPDATE claim set claimdate = NEW.returndate where bookid = NEW.bookid;
    RETURN NEW;
   END IF;
    END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;

CREATE TRIGGER updating_returnH_trigger
AFTER INSERT ON returnhistoryinfo FOR EACH ROW EXECUTE PROCEDURE update_returnH_trigg_function();


CREATE OR REPLACE FUNCTION update_rating_trigg_function() RETURNS trigger AS $BODY$
	DECLARE
	    old_avg_rating numeric;
	    new_avg_rating numeric;
	    no_rating numeric;
	BEGIN
	    IF (TG_TABLE_NAME = 'rate') THEN
		  RAISE NOTICE 'TRIGER called on %', TG_TABLE_NAME;
	    END IF;
	    SELECT rate into old_avg_rating from booksinfo where bookid = NEW.bookid ;
	    SELECT no_ratings into no_rating from booksinfo where bookid = NEW.bookid ;
	    IF (TG_OP = 'INSERT') THEN   
			    new_avg_rating := ((old_avg_rating * no_rating) + NEW.rate)/(no_rating+1);
			    no_rating := no_rating+1;
			   UPDATE booksinfo set rate = new_avg_rating,no_ratings=no_rating where bookid = NEW.bookid ;
			   RETURN NEW;
	    ELSIF (TG_OP = 'UPDATE') THEN
		  IF (NEW.rate < 0) THEN
			RAISE EXCEPTION 'Can''t rate Available balance: %';
		  END IF;
		  IF NEW.rate != OLD.rate THEN
			new_avg_rating := ((old_avg_rating * no_rating)-OLD.rate + NEW.rate)/(no_rating);
			
			EXECUTE 'UPDATE booksinfo(bookid,subject,title,authors,publishers,available,edition,arrivaldate,isbn,similarbooks,price,rate,shelfno,no_ratings)
				  set rate = new_avg_rating,no_ratings=no_rating where bookid = $1.bookid ;' USING NEW, OLD;
		  END IF;
		  RETURN NEW;
	 
	    ELSIF (TG_OP = 'DELETE') THEN
			    new_avg_rating := ((old_avg_rating * no_rating)-OLD.rate )/no_rating -1;
			    no_rating := no_rating -1;
			EXECUTE 'UPDATE booksinfo(bookid,subject,title,authors,publishers,available,edition,arrivaldate,isbn,similarbooks,price,rate,shelfno,no_ratings)
				  set rate = new_avg_rating,no_ratings=no_rating where bookid = NEW.bookid ;' USING NEW,OLD;
			RETURN OLD;
	 
	    END IF;
	 
	    RETURN null;
	END;
$BODY$ LANGUAGE plpgsql VOLATILE COST 100;
CREATE TRIGGER updating_rating_trigger BEFORE INSERT OR UPDATE OR DELETE ON rate FOR EACH ROW EXECUTE PROCEDURE update_rating_trigg_function();

--~ CREATE OR REPLACE FUNCTION update_claim_trigg_function() RETURNS trigger AS $BODY$
	--~ DECLARE
	    --~ no_claim integer;
	--~ BEGIN
	    --~ IF (TG_TABLE_NAME = 'claim') THEN
		  --~ RAISE NOTICE 'TRIGER called on %', TG_TABLE_NAME;
	    --~ END IF;
	    --~ SELECT count(*) into no_claim from claim where bookid = NEW.bookid ;
	    --~ IF (no_claim <3) THEN 
			--~ RETURN NEW;
	    --~ END IF;
	 --~ 
	    --~ RETURN null;
	--~ END;
--~ $BODY$ LANGUAGE plpgsql VOLATILE COST 100;
--~ 
--~ CREATE TRIGGER updating_claim_trigger BEFORE INSERT ON claim FOR EACH ROW EXECUTE PROCEDURE update_claim_trigg_function();

CREATE OR REPLACE FUNCTION update_claim_trigg_function()
  RETURNS trigger AS
$BODY$
DECLARE
    no_claim integer;
    available1 boolean;
BEGIN
    IF (TG_TABLE_NAME = 'claim') THEN
        RAISE NOTICE 'TRIGER called on claim %', TG_TABLE_NAME;
    END IF;
    SELECT count(*) into no_claim from claim where bookid = NEW.bookid ;
    SELECT available into available1 from booksinfo where bookid = NEW.bookid ;
    IF (TG_OP = 'INSERT') THEN
    IF (no_claim+1 <4) THEN 
            RETURN NEW;
    END IF;
    END IF;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;

CREATE TRIGGER updating_claim_trigger
BEFORE INSERT ON claim
FOR EACH ROW
EXECUTE PROCEDURE update_claim_trigg_function();


CREATE OR REPLACE FUNCTION update_issue_trigg_function()
  RETURNS trigger AS
$BODY$
DECLARE
  
BEGIN
    UPDATE booksinfo set available = 'f' where booksinfo.bookid=NEW.bookid;
    RETURN NEW;
    END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;

CREATE TRIGGER updating_issue_triggerM
BEFORE INSERT ON borrowinfoformember FOR EACH ROW EXECUTE PROCEDURE update_issue_trigg_function();

CREATE TRIGGER updating_issue_triggerE
BEFORE INSERT ON borrowinfoforemployee FOR EACH ROW EXECUTE PROCEDURE update_issue_trigg_function();
