
#include "state.h"
#include <string>

state::state( std::initializer_list< unsigned int > init ):sticks(init)
{ }

state::state( unsigned int i1, unsigned int i2 )
{ 
   for (unsigned int i = i1; i <= i2; i++){
      sticks.push_back(i);
   }
}

unsigned int state::sum( ) const 
{
   unsigned int total = 0;
   for (auto stick : sticks){
      total += stick;
   }
   return total;
}

std::optional< move > state::randommove( ) const 
{ 
   if (sum () != 0){
      int row;
      bool flag = true;
      while(flag) {
         row = rand() % (int)sticks.size();
         if (sticks[row] > 0){
            flag = false;
         }
      }
      return move(row, rand() % sticks[row]);
   }else{
      return {};
   }
}


std::optional< move >
state::usermove( std::ostream& out, std::istream& in ) const
{
   // It seems to me that usermove( ) is the same as randommove( ),
   // with the only difference that rand( ) should be replaced by
   // user( ). We could make it into a template. 

   if( sum( ))
   {
      while( true )
      {
         std::string inp;
         try
         {
            out << "row? "; out. flush( );
            in >> inp; 

            unsigned int row = std::stoul( inp ); inp. clear( );

            if( row > 0 && row <= sticks. size( )) 
            {
               unsigned int row1 = row - 1;  // This is a type conversion! 
               if( sticks[ row1 ] )
               {
                  out << "how many remaining? "; out. flush( );
                  in >> inp; 
                  unsigned int remain = std::stoul( inp ); inp. clear( ); 

                  if( remain < sticks[ row1 ] )
                     return move( row1, remain );
                  else
                  {
                     out << "having " << remain 
                         << " remaining is not possible, "
                         << "you have to remove at least one stick\n";
                  }
               }
               else 
               {
                  out << "row " << row << " is already empty\n";
               }
            }
            else
            {
               out << "row " << row << " is not a valid row, ";
               out << "rows are between 1 and " << sticks. size( ) << "\n";
            }
            
         }
         catch( std::invalid_argument& inv )
         {
            out << inp << " is not a number\n";
         }
         catch( std::out_of_range& r )
         {
            out << inp << " is too big\n";
         }
      }
   }
   else
      return std::optional< move > ( );
}


void state::make( move m )
{
   sticks[m.row] = m.remaining;
}


void state::print( std::ostream& out ) const
{ 
   for (size_t row = 0; row < sticks.size(); ++row) {
   out << (row + 1) << " : ";
   for (unsigned int stick = 0; stick < sticks[row]; ++stick) {
      out << "1";
   }
   out << std::endl;
   }
}

std::optional< move> state::removelastmorethantwo( ) const
{
   size_t rowsMoreThanTwo = 0;
   size_t rowToModify = 0;
   for (size_t i = 0; i < sticks.size(); ++i) {
      if (sticks[i] > 1){
         ++rowsMoreThanTwo;
         rowToModify = i;
      }
   }

   if (rowsMoreThanTwo != 1) return std::nullopt;

   size_t rowsOneStick = 0;
   for (size_t stick : sticks) {
      if (stick == 1) {
         ++rowsOneStick;
      }
   }

   size_t sticksToRemove = sticks[rowToModify] - ((rowsOneStick % 2 == 0) ? 1 : 2);

   return move(rowToModify, sticksToRemove);
}

unsigned int state::nimber( ) const
{
   unsigned int ans = 0;
   for (auto stick : sticks){
      ans ^= stick;
   }
   return ans;
}

std::optional<move> state::makenimberzero( ) const
{
   if(nimber() <= 0) 
      return std::nullopt;
   std::vector<int> v;
   size_t index = 0;
   for (auto stick : sticks){
      if (stick) {
         v.push_back(index);
      }
      ++index;
   }
   while(true){
      size_t ind = v[rand() % v.size()];
      for(unsigned int i = 0; i < sticks[ind]; i++){
         if((nimber() ^ sticks[ind] ^ i) == 0){
            return move(ind, i);
         }
      }
   }
}
