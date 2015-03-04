with Ada.Text_IO, Ada.Integer_Text_IO, Ada.Numerics.Float_Random;
use  Ada.Text_IO, Ada.Integer_Text_IO, Ada.Numerics.Float_Random;

procedure Exercise8 is
   
   Count_Failed    : exception;    -- Exception to be raised when counting fails
   Gen             : Generator;    -- Random number generator
   
   protected type Transaction_Manager (N : Positive) is
      entry Finished;
      entry Wait_Until_Aborted;

      procedure Signal_Abort;
   private
      Finished_Gate_Open  : Boolean := False;
      Aborted             : Boolean := False;
   end Transaction_Manager;
   protected body Transaction_Manager is
      entry Finished when Finished_Gate_Open or Finished'Count = N is
      begin

      	Finished_Gate_Open := true;
      	
      	if Finished'Count = 0 Then
      		Finished_Gate_Open := False;
		Aborted := false;
      	end if;
      
    	 ------------------------------------------
	 -- PART 3: Complete the exit protocol here
	 ------------------------------------------
      end Finished;
      
      entry Wait_Until_Aborted when Aborted is 
      begin
	 	 	 
	 if Wait_Until_Aborted'Count = 0 then
	    Put_Line("All workers are aborted");
	 end if;

      end Wait_Until_Aborted;
      
      procedure Signal_Abort is
      begin
	 Aborted := True;
      end Signal_Abort;

      
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
	 select 
	    Manager.Wait_Until_Aborted; --trigger
	    Num := Prev + 5;
	 then abort -- abortable part
	    Put_Line ("Worker" & Integer'Image(Initial) & " started round" & Integer'Image(Round_Num) & ", Value: " & Integer'Image(Num));
	    Round_Num := Round_Num + 1;
	    begin
	       Num := Unreliable_Slow_Add(Num);
	    exception
	       when Count_Failed =>
		  Put_Line("Count failed, worker " & Integer'Image(Initial));
		  Manager.Signal_Abort;
		  
	    end;	    
	 end select;

	 Manager.Finished;
	 
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
end Exercise8;
