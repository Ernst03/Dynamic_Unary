/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * Author: Ernst Berg
 * Contact: ernst@eberg.us with forum support at 'The Odd Duck Din', URL: https://theoddduckdin.freeforums.net/thread/20/ask-guy-discovered-dynamic-unary 
 *
 * Created: January 2010 updated to version two March 5th, 2024
 *
 * Function Overview:
 * The C2P_GMP_v2() function implements a complete encode and decode mechanism for dynamic unary encoding, 
 * supporting all bit position parity references. This approach generates disjoint cycles which are unique number sets 
 * for finite length binary data, facilitating efficient operations with logical functions like XOR.
 * As dynamic numbers exhibit uniform behavior with XOR cycles, this method underscores the versatility 
 * of dynamic unary numbers in computational applications.
 * 
 * The name 'Code to Parity' (C2P) honors the function's origins in dynamic unary encoding, maintaining tradition 
 * while embracing innovation in numerical computations. The "Garden of DU" invites exploration into this fertile 
 * domain, offering new insights into the applications of dynamic numbers.
 */

// Must include stdint.h gmp.h stdbool.h
#include <stdint.h>
#include <gmp.h>
#include <stdbool.h>

struct C2PGMP_v2 {

        mpz_t Input, Output, Reference_Value;
        uint64_t Rotations, DU_Length, Parity_Reference_Pos;
        bool Direction;
};

int C2P_GMP_v2( struct C2PGMP_v2 *Object ) // Dynamic Unary 
{
        // SOL == State of Logic also known as Parity
        uint64_t a,b,c,d, Unary_Int = 0, Write_Pos = 0, Read_Pos;
        bool Flag = false, PRef, Terminus_SOL, Current_Bit, Read_SOL, Write_SOL;
        if( Object->Rotations == 0 ){ fprintf(stderr, "Rotations must be greater than Zero\n"); return(EXIT_FAILURE); }
        if( Object->DU_Length == 0 ){ fprintf(stderr,"Length of binary cannot be Zero\n"); return(EXIT_FAILURE); }
        b =  Object->DU_Length - 1; // For Effeciency.
        if( Object->Parity_Reference_Pos )
        { fprintf(stderr,"Parity sampling position exceeds binary string (n-bit) length\n"); return(EXIT_FAILURE); }

        for( ; Object->Rotations > 0; --(Object->Rotations) ) // Master itteration control over both Encode and Decode
        {
                mpz_set_ui(Object->Output,0); // Clear the write space.
                Read_SOL = mpz_tstbit( Object->Input, 0 ); // Valid for both Encode and Decode.
                Terminus_SOL = mpz_tstbit( Object->Input, Object->Parity_Reference_Pos ); // Can be any position from b(0) to b(DU_Length-1)

                if( Object->Direction == true ) // The Encode direction
                {
                        Write_SOL = !Terminus_SOL; // writing in Terminated Unary: 
                        for( Read_Pos = 0; ; )
                        {
                                for( ; ; ) //Get the count of same parity bits in a row (run) : Unary_Int
                                {
                                        Current_Bit = mpz_tstbit( Object->Input, Read_Pos ); 
                                        if( Current_Bit == Read_SOL ) { Unary_Int++; Read_Pos++; }
                                        else { Read_SOL = !Read_SOL; break; }
                                        if( Read_Pos == Object->DU_Length ) { Flag = true; break; }
                                } // Endo Get Count
                                for( ; Unary_Int != 0 ; --Unary_Int )
                                {
                                        if( Terminus_SOL == false & Unary_Int > 1 ) mpz_setbit( Object->Output, Write_Pos );
                                        if( Unary_Int == 1 & Terminus_SOL == true ) mpz_setbit( Object->Output, Write_Pos );
                                        Write_Pos++;
                                }
                                if( Flag ) break;

                        }
                }
                else  // The Decode direction
                {
                        Flag = false;
                        // Determine the starting state of logic ( starting parity ) for writing.
                        Terminus_SOL = mpz_tstbit( Object->Input, b ); // Read the Terminus from last bit.
                        if( Object->Parity_Reference_Pos > 0 )  // If b0 then Terminus is strting parity
                                for( Write_SOL = false, a = 0; a < Object->DU_Length; a++ ) // Assume starting Write_SOL and search. 
                                {
                                        if( mpz_tstbit( Object->Input, a ) == Terminus_SOL ) // Read by terminated Unary
                                        {
                                                if( Object->Parity_Reference_Pos <= a ) // Is the reference position in this Unary integer?
                                                {
                                                        if( Write_SOL != Terminus_SOL ) Write_SOL = !Write_SOL; // Change starting Write_SOL if:
                                                        break;
                                                }
                                                else Write_SOL = !Write_SOL; // Else switch SOL and continue search.
                                        }
                                }
                        else Write_SOL = Terminus_SOL; // Write_SOL is parity at Terminus (EOS)
                                                       // Write runs of same parity bits per terminated unary integer length

                        for( Read_Pos = 0 ; Flag != true; )
                        {
                                for( ; ; ) // Read terminated Unary and write that count of same parity bits in alternating pattern.
                                {
                                        if( Read_Pos == b ) Flag = true;
                                        Current_Bit = mpz_tstbit( Object->Input, Read_Pos++ );
                                        Unary_Int++;
                                        if( Current_Bit == Terminus_SOL ) break; // EOS : 

                                }

                                for( ; ; ) // Write the bits out
                                {
                                        if( Write_SOL == true ) mpz_setbit( Object->Output, Write_Pos );
                                        Write_Pos++;
                                        printf("Write_Pos is %lu\n",Write_Pos);
                                        if( --Unary_Int == 0 ) { Write_SOL = !Write_SOL; break; } // Flip write logic

                                }
                        }
			 if( Object->Rotations > 1 )
                        {
                                mpz_set( Object->Input, Object->Output );
                        }
                }
        } // Endo Rotations

}// Endo C2P_GMP_v2

void Init_C2PGMP_v2( struct C2PGMP_v2 *Object )
{
        mpz_inits( Object->Input, Object->Output, Reference_Value, NULL );
}

void Free_C2PGMP_v2( struct C2PGMP_v2 *Object )
{
        mpz_clears( Object->Input, Object->Output, Reference_Value, NULL );
}

