/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "DSP.h"
#include "main.h"

/*******************************************************************************
* Function Name: void sort(int16 * ArrayIn, uint8 N)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void sort(int16 * ArrayIn, uint8 N)
{
    int i, j;
	int16 temp;
    for (i=N-1;i>=0;i--)
    {
        for(j=1;j<=i;j++)
        {
            if (ArrayIn[j-1] > ArrayIn[j])
            {
                temp = ArrayIn[j-1];
                ArrayIn[j-1] = ArrayIn[j];
                ArrayIn[j] = temp;
            }
        }
    }
}



/*******************************************************************************
* Function Name: int16 Med_Filter(int16 * ArrayIn, uint8 W)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
int16 Med_Filter(int16 * ArrayIn, uint8 W)
{
	int16 FilterResult;
	switch(W)
	{
		case 3:
		{
			FilterResult = medfilt3(ArrayIn);
			break;	
		}
		case 5:
		{
			FilterResult = medfilt5(ArrayIn);
			break;	
		}
		case 6:
		{
			FilterResult = medfilt6(ArrayIn);
			break;	
		}
		case 7:
		{
			FilterResult = medfilt7(ArrayIn);
			break;	
		}
		case 9:
		{
			FilterResult = medfilt9(ArrayIn);
			break;	
		}
		case 25:
		{
			FilterResult = medfilt25(ArrayIn);
			break;	
		}		
		default:
		{
			FilterResult = ArrayIn[0];
			break;
		}
	}
	return(FilterResult);
	//sort(ArrayIn, W);
	//return(ArrayIn[((W+1)/2)]);
}

/*******************************************************************************
* Function Name: float SingelPole_IIR_LPF(float x, float y, float Fc, float Fs)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/

    /* IIR filter:

     * Difference equation of 1-pole IIR
     y[k+1] = y[k] + (x[k]-y[k])/a

     * The Z transform of the transfer function:
     H(z)=1/[a+z^(-1)*(1-a)]

     * The cutoff frequency (Fo) at a sampling rate Fs:
     Fo = Fs / 2*(pi)*a
     a = 256/1
     Fs = 100Hz
     Fo = 100 /2*(pi)*256

     **************
     ** Fo = 0.06Hz **
     **************

     * Settling Time (Ts):
     s[n] = 1 - (1- 1/a )^n
     n = ln(0.001) / ln(1-1/a)
     n = 1765
     Ts = n/Fs

     ********************
     ** Ts = = 17.6sec **
     ********************
     */
float SingelPole_IIR_LPF(float x, float y, float Fc, float Fs)
{
    return (y = y + (x - y)*OMEGA*Fc/Fs);
}

int32 IIR_LPF_sp(int32 input, int32 * filt, uint8 Numerator)
{
     /* 
	
	Numerator = round(512*PI*Fc/Fs)
	Final Fc = Fs/(2*PI*numerator)
	
	Settling Time (Ts):
     s[n] = 1 - (1- 1/a )^n
     n = ln(0.001) / ln(1-Numarator/256)
     Ts = n/Fs
	
	*/
	input <<= 8;
	
	*filt = *filt + (((input-*filt) >> 8) * Numerator);

	return ((*filt>>8) + ((*filt & 0x00000080) >> 7));
}

/*******************************************************************************
* Function Name: Shift_And_Store_int16(int16 * pArray, uint8 SizeOfArray, int16 NewData, char ShiftDirection);
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Shift_And_Store_int16(int16 * pArray, uint8 SizeOfArray, int16 NewData, char ShiftDirection)
{
	uint8 k;
	switch(ShiftDirection)
	{
		case 'R':
		{
		    for (k = (SizeOfArray-1); k > 0; k--)
		    {
		        pArray[k] = pArray[k-1];
		    }			
			pArray[0] = NewData;
			break;
		}
		case 'L':
		{
		    for (k = 0; k < SizeOfArray-1; k++)
		    {
		        pArray[k] = pArray[k+1];
		    }			
			pArray[SizeOfArray-1] = NewData;			
			break;
		}		
	}
}

/*******************************************************************************
* Function Name: uint16 findpeaks(uint16 * Data, uint8 N, uint8 aw)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
uint16 findpeaks(uint16 * Data, uint8 N, uint8 aw)
{
	uint32	Peak;
	uint32	Sum;
	uint8	i;
	int8	j;
	/* Input protection */
	if (aw<1)
	{
		aw=1;	
	}
	if (N<1)
	{
		N=1;	
	}	
	
	Peak = 0;
	
	for(i=0;i<N;i++)
	{
		Check_Preemption();
		Sum = 0;
	    if ((i+1) < aw)
		{
			for(j=i;j>=0;j--)
			{
				Sum += (uint32) Data[j]; 	
			}
		}
		else
		{
			for(j=i;j>i-aw;j--)
			{
				Sum += (uint32) Data[j]; 	
			}			
		}
		if(Sum > Peak)
		{
			Peak = Sum;	
		}
	}
	return( (int16) (Peak/aw));
}

/*******************************************************************************
* Function Name: int16 mean(int16 * Data, uint8 N)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.ADC_CountsTo_Volts()
*******************************************************************************/
int16 mean(int16 * Data, uint8 N)
{
	uint8 i;
	int32 mean = 0;
	
	for(i=0;i<N;i++)
	{
		mean += Data[i];
	}
	mean /= N;
	
	return( (int16) mean);
}

/*******************************************************************************
* Function Name: int16 RangeMean(int16 * Data, uint8 N, uint8 R)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.ADC_CountsTo_Volts()
*******************************************************************************/
int16 RangeMean(int16 * Data, uint8 N, uint8 R)
{
	uint8 i;
	int32 mean = 0;
	
	for(i=0;i<N;i++)
	{
		if(!i)
		{
			mean += Data[i];	
		}
		else
		{
			if( fabs(Data[i] - Data[i-1]) < R)
			{
				mean += Data[i];
			}
			else
			{
				Data[i] = Data[i-1];
				mean += Data[i];
			}
		}
		
	}
	mean /= N;
	
	return( (int16) mean);
}

/*******************************************************************************
* Function Name: uint16 accum(uint16 * Data, uint8 N)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   
*******************************************************************************/
uint16 accum(uint16 * Data, uint8 N)
{
	uint8 i;
	uint32 accum = 0;
	
	for(i=0;i<N;i++)
	{
		accum += Data[i];
	}
	
	return( (uint16) accum);
}

/*----------------------------------------------------------------------------
   Function :   opt_med3()
   In       :   pointer to array of 3 pixel values
   Out      :   a int16
   Job      :   optimized search of the median of 3 pixel values
   Notice   :   found on sci.image.processing
                cannot go faster unless assumptions are made
                on the nature of the input signal.
 ---------------------------------------------------------------------------*/
int16 medfilt3(int16 * p)
{
    PAIR_SORT(p[0],p[1]) ; PAIR_SORT(p[1],p[2]) ; PAIR_SORT(p[0],p[1]) ;
    return(p[1]) ;
}

/*----------------------------------------------------------------------------
   Function :   opt_med5()
   In       :   pointer to array of 5 pixel values
   Out      :   a int16
   Job      :   optimized search of the median of 5 pixel values
   Notice   :   found on sci.image.processing
                cannot go faster unless assumptions are made
                on the nature of the input signal.
 ---------------------------------------------------------------------------*/
int16 medfilt5(int16 * p)
{
    PAIR_SORT(p[0],p[1]) ; PAIR_SORT(p[3],p[4]) ; PAIR_SORT(p[0],p[3]) ;
    PAIR_SORT(p[1],p[4]) ; PAIR_SORT(p[1],p[2]) ; PAIR_SORT(p[2],p[3]) ;
    PAIR_SORT(p[1],p[2]) ; return(p[2]) ;
}

/*----------------------------------------------------------------------------
   Function :   opt_med6()
   In       :   pointer to array of 6 pixel values
   Out      :   a int16
   Job      :   optimized search of the median of 6 pixel values
   Notice   :   from Christoph_John@gmx.de
                based on a selection network which was proposed in
                "FAST, EFFICIENT MEDIAN FILTERS WITH EVEN LENGTH WINDOWS"
                J.P. HAVLICEK, K.A. SAKADY, G.R.KATZ
                If you need larger even length kernels check the paper
 ---------------------------------------------------------------------------*/
int16 medfilt6(int16 * p)
{
    PAIR_SORT(p[1], p[2]); PAIR_SORT(p[3],p[4]);
    PAIR_SORT(p[0], p[1]); PAIR_SORT(p[2],p[3]); PAIR_SORT(p[4],p[5]);
    PAIR_SORT(p[1], p[2]); PAIR_SORT(p[3],p[4]);
    PAIR_SORT(p[0], p[1]); PAIR_SORT(p[2],p[3]); PAIR_SORT(p[4],p[5]);
    PAIR_SORT(p[1], p[2]); PAIR_SORT(p[3],p[4]);
    return ( p[2] + p[3] ) * 0.5;
    /* PAIR_SORT(p[2], p[3]) results in lower median in p[2] and upper median in p[3] */
}


/*----------------------------------------------------------------------------
   Function :   opt_med7()
   In       :   pointer to array of 7 pixel values
   Out      :   a int16
   Job      :   optimized search of the median of 7 pixel values
   Notice   :   found on sci.image.processing
                cannot go faster unless assumptions are made
                on the nature of the input signal.
 ---------------------------------------------------------------------------*/
int16 medfilt7(int16 * p)
{
    PAIR_SORT(p[0], p[5]) ; PAIR_SORT(p[0], p[3]) ; PAIR_SORT(p[1], p[6]) ;
    PAIR_SORT(p[2], p[4]) ; PAIR_SORT(p[0], p[1]) ; PAIR_SORT(p[3], p[5]) ;
    PAIR_SORT(p[2], p[6]) ; PAIR_SORT(p[2], p[3]) ; PAIR_SORT(p[3], p[6]) ;
    PAIR_SORT(p[4], p[5]) ; PAIR_SORT(p[1], p[4]) ; PAIR_SORT(p[1], p[3]) ;
    PAIR_SORT(p[3], p[4]) ; return (p[3]) ;
}

/*----------------------------------------------------------------------------
   Function :   opt_med9()
   In       :   pointer to an array of 9 int16s
   Out      :   a int16
   Job      :   optimized search of the median of 9 int16s
   Notice   :   in theory, cannot go faster without assumptions on the
                signal.
                Formula from:
                XILINX XCELL magazine, vol. 23 by John L. Smith
  
                The input array is modified in the process
                The result array is guaranteed to contain the median
                value
                in middle position, but other elements are NOT sorted.
 ---------------------------------------------------------------------------*/
int16 medfilt9(int16 * p)
{
    PAIR_SORT(p[1], p[2]) ; PAIR_SORT(p[4], p[5]) ; PAIR_SORT(p[7], p[8]) ;
    PAIR_SORT(p[0], p[1]) ; PAIR_SORT(p[3], p[4]) ; PAIR_SORT(p[6], p[7]) ;
    PAIR_SORT(p[1], p[2]) ; PAIR_SORT(p[4], p[5]) ; PAIR_SORT(p[7], p[8]) ;
    PAIR_SORT(p[0], p[3]) ; PAIR_SORT(p[5], p[8]) ; PAIR_SORT(p[4], p[7]) ;
    PAIR_SORT(p[3], p[6]) ; PAIR_SORT(p[1], p[4]) ; PAIR_SORT(p[2], p[5]) ;
    PAIR_SORT(p[4], p[7]) ; PAIR_SORT(p[4], p[2]) ; PAIR_SORT(p[6], p[4]) ;
    PAIR_SORT(p[4], p[2]) ; return(p[4]) ;
}


/*----------------------------------------------------------------------------
   Function :   opt_med25()
   In       :   pointer to an array of 25 int16s
   Out      :   a int16
   Job      :   optimized search of the median of 25 int16s
   Notice   :   in theory, cannot go faster without assumptions on the
                signal.
  				Code taken from Graphic Gems.
 ---------------------------------------------------------------------------*/
int16 medfilt25(int16 * p)
{


    PAIR_SORT(p[0], p[1]) ;   PAIR_SORT(p[3], p[4]) ;   PAIR_SORT(p[2], p[4]) ;
    PAIR_SORT(p[2], p[3]) ;   PAIR_SORT(p[6], p[7]) ;   PAIR_SORT(p[5], p[7]) ;
    PAIR_SORT(p[5], p[6]) ;   PAIR_SORT(p[9], p[10]) ;  PAIR_SORT(p[8], p[10]) ;
    PAIR_SORT(p[8], p[9]) ;   PAIR_SORT(p[12], p[13]) ; PAIR_SORT(p[11], p[13]) ;
    PAIR_SORT(p[11], p[12]) ; PAIR_SORT(p[15], p[16]) ; PAIR_SORT(p[14], p[16]) ;
    PAIR_SORT(p[14], p[15]) ; PAIR_SORT(p[18], p[19]) ; PAIR_SORT(p[17], p[19]) ;
    PAIR_SORT(p[17], p[18]) ; PAIR_SORT(p[21], p[22]) ; PAIR_SORT(p[20], p[22]) ;
    PAIR_SORT(p[20], p[21]) ; PAIR_SORT(p[23], p[24]) ; PAIR_SORT(p[2], p[5]) ;
    PAIR_SORT(p[3], p[6]) ;   PAIR_SORT(p[0], p[6]) ;   PAIR_SORT(p[0], p[3]) ;
    PAIR_SORT(p[4], p[7]) ;   PAIR_SORT(p[1], p[7]) ;   PAIR_SORT(p[1], p[4]) ;
    PAIR_SORT(p[11], p[14]) ; PAIR_SORT(p[8], p[14]) ;  PAIR_SORT(p[8], p[11]) ;
    PAIR_SORT(p[12], p[15]) ; PAIR_SORT(p[9], p[15]) ;  PAIR_SORT(p[9], p[12]) ;
    PAIR_SORT(p[13], p[16]) ; PAIR_SORT(p[10], p[16]) ; PAIR_SORT(p[10], p[13]) ;
    PAIR_SORT(p[20], p[23]) ; PAIR_SORT(p[17], p[23]) ; PAIR_SORT(p[17], p[20]) ;
    PAIR_SORT(p[21], p[24]) ; PAIR_SORT(p[18], p[24]) ; PAIR_SORT(p[18], p[21]) ;
    PAIR_SORT(p[19], p[22]) ; PAIR_SORT(p[8], p[17]) ;  PAIR_SORT(p[9], p[18]) ;
    PAIR_SORT(p[0], p[18]) ;  PAIR_SORT(p[0], p[9]) ;   PAIR_SORT(p[10], p[19]) ;
    PAIR_SORT(p[1], p[19]) ;  PAIR_SORT(p[1], p[10]) ;  PAIR_SORT(p[11], p[20]) ;
    PAIR_SORT(p[2], p[20]) ;  PAIR_SORT(p[2], p[11]) ;  PAIR_SORT(p[12], p[21]) ;
    PAIR_SORT(p[3], p[21]) ;  PAIR_SORT(p[3], p[12]) ;  PAIR_SORT(p[13], p[22]) ;
    PAIR_SORT(p[4], p[22]) ;  PAIR_SORT(p[4], p[13]) ;  PAIR_SORT(p[14], p[23]) ;
    PAIR_SORT(p[5], p[23]) ;  PAIR_SORT(p[5], p[14]) ;  PAIR_SORT(p[15], p[24]) ;
    PAIR_SORT(p[6], p[24]) ;  PAIR_SORT(p[6], p[15]) ;  PAIR_SORT(p[7], p[16]) ;
    PAIR_SORT(p[7], p[19]) ;  PAIR_SORT(p[13], p[21]) ; PAIR_SORT(p[15], p[23]) ;
    PAIR_SORT(p[7], p[13]) ;  PAIR_SORT(p[7], p[15]) ;  PAIR_SORT(p[1], p[9]) ;
    PAIR_SORT(p[3], p[11]) ;  PAIR_SORT(p[5], p[17]) ;  PAIR_SORT(p[11], p[17]) ;
    PAIR_SORT(p[9], p[17]) ;  PAIR_SORT(p[4], p[10]) ;  PAIR_SORT(p[6], p[12]) ;
    PAIR_SORT(p[7], p[14]) ;  PAIR_SORT(p[4], p[6]) ;   PAIR_SORT(p[4], p[7]) ;
    PAIR_SORT(p[12], p[14]) ; PAIR_SORT(p[10], p[14]) ; PAIR_SORT(p[6], p[7]) ;
    PAIR_SORT(p[10], p[12]) ; PAIR_SORT(p[6], p[10]) ;  PAIR_SORT(p[6], p[17]) ;
    PAIR_SORT(p[12], p[17]) ; PAIR_SORT(p[7], p[17]) ;  PAIR_SORT(p[7], p[10]) ;
    PAIR_SORT(p[12], p[18]) ; PAIR_SORT(p[7], p[12]) ;  PAIR_SORT(p[10], p[18]) ;
    PAIR_SORT(p[12], p[20]) ; PAIR_SORT(p[10], p[20]) ; PAIR_SORT(p[10], p[12]) ;

    return (p[12]);
}
/* [] END OF FILE */
















