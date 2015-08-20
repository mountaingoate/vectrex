/*

	Copyright (c) 2015 Mountain Goat Entertainment.
	All rights reserved.
	
	Redistribution and use in source and binary forms are permitted
	provided that the above copyright notice and this paragraph are
	duplicated in all such forms and that any documentation,
	advertising materials, and other materials related to such
	distribution and use acknowledge that the software was developed
	by Mountain Goat Entertainment. The name of  Mountain Goat 
	Entertainment may not be used to endorse or promote products derived
	from this software without specific prior written permission.
	THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

// Implement a rotation for Vectrex vector lists to precompute "Rot_VL_ab"
// This way instead of calling Rot_VL_ab one can just index into an array of frames

public class VecRot {

	
	// Disable the constructor
	private VecRot()
	{
	}
	
	// Output assembly to stdout - it can be directly included in a Vectrex .asm file
	public static void Rotate(String name, int [] vl)
	{
		// The rotation matrix for the Vectrex is:
		// [ [ cos(a), sin(a) ] [ -sin(a), cos(a) ] ]
		// This is because the rotation "starts" on the Y axis
		
		// The Vectrex uses an angle representation of 0 - 64 for 0 - 360 degrees,
		// one Vectrex angle = 5.625 degrees
		double unitAngle = 5.625;
		
		// Print some help info
		System.out.println("; How to use - assume Vectrex angle (0-63) is stored in " + name + "Angle");
		System.out.println(";	lda		" + name + "Angle");
		System.out.println(";	asla		; addresses are 2 byte words");
		System.out.println(";	ldx		#" + name + "_vector_frames");
		System.out.println(";	ldx		a,x");
		System.out.println(";	lda		#" + name.toUpperCase() + "_LIST_LEN - 1");
		System.out.println(";	jsr		Draw_VL_a");
		System.out.println();
		
		
		System.out.println(name.toUpperCase() + "_LIST_LEN		equ " + vl.length / 2 + "\n");		
		System.out.println(name + "_vector_frames:");
		
		for (int angleIdx = 0; angleIdx < 64; angleIdx++)
		{
			System.out.println("		dw	" + name + "_vector_list_" + angleIdx);
		}
		
		for (int angleIdx = 0; angleIdx < 64; angleIdx++)
		{
			System.out.println("\n" + name + "_vector_list_" + angleIdx + ":");
			
			double angle = unitAngle * angleIdx;
			
			double rad = angle * Math.PI / 180.0;
			//System.out.println("RAD " + rad);
			
			// The Vectrex uses relative coordinates in its vector lists, we need absolute
			// Original absolute position
			double x0 = 0.0;
			double y0 = 0.0;
			
			// Rotated absolute position
			double xr0 = 0.0;
			double yr0 = 0.0;
			
			for (int i = 0; i < vl.length; i += 2)
			{	
				double y = y0 + (double)vl[i];
				double x = x0 + (double)vl[i + 1];
				
				// A benefit of doing this math on doubles is that rotated vector lists will not diverge
				//  which can happen on a real Vectrex when using Rot_VL_ab
				double yr = y * Math.cos(rad) + x * Math.sin(rad);
				double xr = -1 * y * Math.sin(rad) + x * Math.cos(rad);
				
				int dy = (int) Math.round(yr - yr0);
				int dx = (int) Math.round(xr - xr0);
				
				if ((dy < -128) || (dy > 127) || (dx < -128) || (dx > 127)) {
					System.out.println(";  WARNING COORDINATE TOO BIG!");
				}
				
				System.out.println("		db " + Math.round(yr - yr0) + ", " + Math.round(xr - xr0));
				
				y0 = y;
				x0 = x;
				
				yr0 = yr;
				xr0 = xr;
			}
		}
	}
	
	// Example use:
	public static void main(String[] args) {
		int [] vl = { -10, -3, 0, 6, 10, -3 };
		
		Rotate("triangle", vl);
	}

}
