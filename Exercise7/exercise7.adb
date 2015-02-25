with Ada.Text_IO, Ada.Integer_Text_IO, Ada.Numerics.Float_Random;
use  Ada.Text_IO, Ada.Integer_Text_IO, Ada.Numerics.Float_Random;

procedure Exercise7 is
   
   Count_Failed    : exception;    -- Exception to be raised when counting fails
   Gen             : Generator;    -- Random number generator
   
   protected type Transaction_Manager (N : Positive) is
      entry Finished;
      function Commit return Boolean;
      procedure Signal_Abort;
   private
      Finished_Gate_Open  : Boolean := False;
      Aborted             : Boolean := False;
      Should_Commit       : Boolean := True;
   end Transaction_Manager;
   protected body Transaction_Manager is
      entry Finished when Finished_Gate_Open or Finished'Count = N is
      begin

      	Finished_Gate_Open := true;
      	
      	if Finished'Count = 0 Then
      		Finished_Gate_Open := False;
      		if Aborted = true Then
      			Should_Commit := false;
      		else
      			Should_Commit := True;
      		end if;
      		Aborted := false;
      	end if;
      


	 ------------------------------------------
	 -- PART 3: Complete the exit protocol here
	 ------------------------------------------
      end Finished;
      
      procedure Signal_Abort is
      begin
	 Aborted := True;
      end Signal_Abort;
      
      function Commit return Boolean is
      begin
	 return Should_Commit;
      end Commit;
      
   end TrAnsAction_Manager;
   
   
   
   
   function Unreliable_Slow_Add (X : Integer) return Integer is
      Error_Rate : constant := 0.15;  -- (between 0 and 1)
   begin
      if Random(Gen) < Error_Rate then -- error occured
	 delay Duration(Random(Gen)*0.5);
	 raise Count_Failed;
      else
	 delay 4.0;
	 return X + 10;
      end if;
   end Unreliable_Slow_Add;
   
   
   
   
   task type Transaction_Worker (Initial : Integer; Manager : access Transaction_Manager);
   task body Transaction_Worker is
      Num         : Integer   := Initial;
      Prev        : Integer   := Num;
      Round_Num   : Integer   := 0;
   begin
      Put_Line ("Worker" & Integer'Image(Initial) & " started");
      
      loop
	 Put_Line ("Worker" & Integer'Image(Initial) & " started round" & Integer'Image(Round_Num));
	 Round_Num := Round_Num + 1;
	 begin
	 	Num := Unreliable_Slow_Add(Num);
	 exception
   		when Count_Failed =>
   			Manager.Signal_Abort;
   			Put_Line("Count failed, worker " & Integer'Image(Initial));
   	 end;
	 Manager.Finished;
	 ---------------------------------------
	 -- PART 2: Do the transaction work here             
	 ---------------------------------------
	 
	 if Manager.Commit = True then
	    Put_Line ("  Worker" & Integer'Image(Initial) & " comitting" & Integer'Image(Num));
	 else
	    Put_Line ("  Worker" & Integer'Image(Initial) &
			" reverting from" & Integer'Image(Num) &
			" to" & Integer'Image(Prev));
	    Num := Prev;
	    -------------------------------------------
	    -- PART 2: Roll back to previous value here
	    -------------------------------------------
	
	 end if;
	 
	 Prev := Num;
	 delay 0.5;
	 
      end loop;
   		
   end Transaction_Worker;
   
   Manager : aliased Transaction_Manager (3);
   
   Worker_1 : Transaction_Worker (0, Manager'Access);
   Worker_2 : Transaction_Worker (1, Manager'Access);
   Worker_3 : Transaction_Worker (2, Manager'Access);
   
begin
   Reset(Gen); -- Seed the random number generator
end Exercise7;
