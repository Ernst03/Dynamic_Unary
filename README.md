# Dynamic Unary

Dynamic Unary is an encoding system implemented in C, leveraging the GNU Multiple Precision Arithmetic Library. This innovative method generates disjoint cycles derived from finite length binary strings, providing dynamic number sets unique to each bit position referenced for all n-bit of that length of binary string. It explores the fascinating intersection of information, cycle, and numerical representation, particularly highlighting unique properties across different lengths of binary sequences. Each unique cycle in that set can be identified by the summation of all elements in that cycle creating an identifier value. Yeah, that was a cool thing, each disjoint cycle summed to the same value. I am sure there are more, interesting qualities to be discovered.

## Interesting Observations

- Length 1 serves as a compelling proof of concept that a single bit cycles, suggesting a fundamental distinction between information and cycle since both parities do not interact for length one bit. The other parity exists as information but not in cycle with the other for length one-bit.
The concept of a single bit cycling unto itself may work as an identity. It could indeed serve as a proof of parity or the existence of parity. If a '0' cycles to '0' and a '1' cycles to '1', it suggests a stable, invariant property: the parity of the bit remains unchanged through the cycle. This invariance can be viewed as an identity property in the realm of dynamic unary operations, where the operation (in this case, cycling) does not alter the element's fundamental attribute (its parity).
- Lengths 2 and beyond, multiple cycles emerge, offering a rich field for exploration and analysis. For a detailed list of all cycles for all reference positions up to and including 8-bit lengths, see my "Flag-Planting" paper on [arXiv.org](https://arxiv.org/pdf/1405.2846.pdf).

## Overview

Dynamic Unary while it can be implemented in encoding constructs, generates dynamic number sets. The encoding approach can be diversified, yet this implementation specifically leverages the state logic of a single bit at a specific bit position supplying the Terminus parity for both encoding and decoding processes. Dynamic Unary Numbers scale from one bit to infinite bit-length and are always parsed in the finite length.  

## How It Works

- **Encoding Process**: Using the Parity_Reference_Position to guide us with which state logic to use for the Terminus parity each encoding step, encoding reads same parity runs that alternate parity (state of logic SOL) in binary strings. Using the Unary form of one parity as body and the other as terminus, an example of writing the count of same parity runs for the string 000 can be either 001 or 110. 
-**Decoding Process**: The Terminus parity is read from the last bit. Then the starting write parity is assumed logically false {0} then the correct starting parity is determined by scanning the unary integers until the Parity_Reference_Position is reached and if the assumed starting parity matches the scan position logic then the starting parity is the assumed parity. If not then the logic is flipped and the starting write parity is reversed. It is then a simple read the Unary Integer and write the correct parity bits in the correct orders.  
  - Example: a binary string sequence like '00011101010' becomes a count of `{3,3,1,1,1,1}`, allowing for accurate encoding or decoding when the starting parity found at the Parity_Reference_Position is known. Example if bit position 0 is the Parity_Reference_Position, Terminus then is 0 and the above decimal set is written as {110,110,0,0,0,0,0} or if bit position 3 is used, Terminus is 1 and written { 001,001,1,1,1,1,1}. Remember each bit position is a unique number set.

- **Key Components**:
- **Input, Output, and Reference_Value**: Of `mpz_t` type, facilitating input, output, and verification of cycle completion when original source is stored for comparison to output. Note: Output is copied to Input to facilitate cycle to the next element so, comparing Output to Reference_Value is the way to check for complete cycle due to some Parity_Reference_Positions and their lengths of binary string generating different element counts per cycle. Comparing Source to Output to determine the end of a cycle is especially important  in more complex constructs involving multiple dynamic unary numbers.
  - **Rotations, DU_Length, and Parity_Reference_Pos**: Essential parameters that define the operation of the encoding/decoding process.
  - **Direction**: Sets the mode of operation, choosing between encoding and decoding.

##Exploring the Universe of Numbers

This function not only advances our understanding of dynamic number sets but also paves the way for a Dynamic Unary Library inclusive of basic mathematical operations like addition, subtraction, multiplication, and division—all through logical operations.

###Next Steps

The journey towards developing a comprehensive library is in motion, aiming to enhance the practical applications of Dynamic Unary Encoding and encouraging the community to delve into the numerical universe's uncharted territories. 
In the age of artificial inelegance I wonder if dynamic numbers will play am important role since they interact with logic. 
 Another question I have asked is could this be related to particle physics since the qualities of full spin and half spin are demonstrated by dynamic unary and it’s dynamic numbers under the logical operation of Xor.

###Closing : It has been a decade since I first published and at that time people wanted code to work with which I did not supply. I now publish code to work with and a support forum to go to for questions and answers. 

###Example Snippet:

// You will include the dynamic unary code C2P_GMP_v2() as well.

struct C2PGMP_v2 {

        mpz_t Input, Output, Reference_Value;
        uint64_t Rotations, DU_Length, Parity_Reference_Pos;
        bool Direction;

};
int main()
{
struct C2PGMP_v2 DU_Object;
bool Encode = true, Decode = false;

              DU_Object.Direction = Encode; // Decode
              DU_Object.Parity_Reference_Pos = 0;
              DU_Object.DU_Length = 64; // For testing.
              DU_Object.Parity_Reference_Pos = 0; // b0 to b63 for a 64-bit length
              mpz_set_ui( DU_Object.Input, 8790876387142254079u ); // Try it with any number or pattern
              mpz_set( DU_Object.Reference_Value, DU_Object.Input );
             
              for( ; ; )
              {
                      DU_Object.Rotations = 1; // Must be set every time as it is decremented in loop
                      printf("Calling C2P : Rotations are %lu\n",DU_Object.Rotations);
                      C2P_GMP_v2( &DU_Object );
                      gmp_printf("Returned value is %Zu\n",DU_Object.Output);
                      GMP_Display_Binary( DU_Object.Output, 64 );
                      if( mpz_cmp( DU_Object.Output, DU_Object.Reference_Value ) == 0 )
                      {
                              printf("Cycle complete\n");
                              break;
                      }
                      else mpz_set( DU_Object.Input, DU_Object.Output );
              }
} // ENDO Main()

void GMP_Display_Binary( mpz_t Data, uint64_t Length )
{
        uint64_t x;

        for( x = 0; x < Length; x++ )
        { if( mpz_tstbit( Data, x ) == 1 ) printf("1"); else printf("0"); }
        printf("\n");

}


