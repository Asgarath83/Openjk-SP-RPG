/*
This file is part of Jedi Academy.

    Jedi Academy is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    Jedi Academy is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Jedi Academy.  If not, see <http://www.gnu.org/licenses/>.
*/
// Copyright 2001-2013 Raven Software

// Disruptor Weapon

// this line must stay at top so the whole PCH thing works...
#include "cg_headers.h"

//#include "cg_local.h"
#include "cg_media.h"
#include "FxScheduler.h"


/*
---------------------------
FX_DisruptorMainShot
---------------------------
*/
static vec3_t WHITE	={1.0f,1.0f,1.0f};

void FX_DisruptorMainShot( vec3_t start, vec3_t end )
{
	FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/redLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}


/*
---------------------------
FX_DisruptorAltShot
---------------------------
*/
void FX_DisruptorAltShot( vec3_t start, vec3_t end, qboolean fullCharge )
{
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/redLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	if ( fullCharge )
	{
		vec3_t	YELLER={0.8f,0.7f,0.0f};

		// add some beef
		FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
	}
}

/*
---------------------------
FX_DisruptorAltMiss
---------------------------
*/

void FX_DisruptorAltMiss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	theFxScheduler.PlayEffect( "disruptor/alt_miss", origin, normal );
}

/*
---------------------------
FX_KothosBeam
---------------------------
*/
void FX_KothosBeam( vec3_t start, vec3_t end )
{
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/misc/dr1" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	YELLER={0.8f,0.7f,0.0f};

	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						YELLER, YELLER, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
}






/*
---------------------------
void RAY SHOOTS GUN LIBRARY CODES
---------------------------
*/

void FX_Ray1_Shot( vec3_t start, vec3_t end )
{
	centity_t	*cent;
	cent = cg_entities;
	  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_CRIMSON )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/crimsonLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_RED )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/redLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ORANGE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/OrangeLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	  else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_YELLOW )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/YellowLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	   else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_BROWN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/BrownLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	   else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_GREEN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/GreenLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	     else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_BLUE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/BlueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_AZURE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/azureLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_CYAN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/cyanLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_INDIGO )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/indigoLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_PINK )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/pinkLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_WHITE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/whiteLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}

	  else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_IRIS )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/irisLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	  else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_GREY )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/greyLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}

	   else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_BLACK )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/blackLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}

	   else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_CRIMSON )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electriccrimsonLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_RED )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricredLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_ORANGE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricOrangeLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	  else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_YELLOW )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricYellowLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	   else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_BROWN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricBrownLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	   else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_GREEN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricGreenLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	     else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_BLUE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricBlueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_AZURE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricazureLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_CYAN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electriccyanLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_INDIGO )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricindigoLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_PINK )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricpinkLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_WHITE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricwhiteLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}

	  else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_GREY )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricgreyLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}

	   else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_BLACK )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricblackLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
}

void FX_Ray1_Alt_Shot( vec3_t start, vec3_t end )
{
	centity_t	*cent;
	cent = cg_entities;
	  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_CRIMSON )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/crimsonLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_RED )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/redLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ORANGE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/OrangeLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	  else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_YELLOW )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/YellowLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	   else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_BROWN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/BrownLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	   else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_GREEN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/GreenLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	     else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_BLUE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/BlueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_AZURE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/azureLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_CYAN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/cyanLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_INDIGO )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/indigoLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_PINK )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/pinkLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_WHITE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/whiteLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}

	  else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_IRIS )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/irisLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	  else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_GREY )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/greyLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}

	   else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_BLACK )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/blackLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}

	   else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_CRIMSON )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electriccrimsonLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_RED )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricredLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_ORANGE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricOrangeLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	  else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_YELLOW )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricYellowLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	   else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_BROWN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricBrownLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	   else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_GREEN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricGreenLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	     else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_BLUE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricBlueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_AZURE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricazureLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_CYAN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electriccyanLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_INDIGO )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricindigoLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_PINK )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricpinkLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_WHITE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricwhiteLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}

	  else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_GREY )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricgreyLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}

	   else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_BLACK )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 4.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							120, cgi_R_RegisterShader( "gfx/effects/electricblackLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	
}


void FX_Ray2_Shot( vec3_t start, vec3_t end, qboolean fullCharge )
{
	centity_t	*cent;
	cent = cg_entities;

	if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_CRIMSON )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/crimsonLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_RED )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/redLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ORANGE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/orangeLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_YELLOW )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/yellowLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_BROWN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/brownLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_GREEN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/greenLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_BLUE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/blueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_AZURE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/azureLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_CYAN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/cyanLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}

		
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_INDIGO )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/indigoLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_PURPLE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/purpleLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_PINK )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/pinkLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_WHITE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/whiteLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


		else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_GREY )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/greyLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_BLACK )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/blackLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_IRIS )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/irisLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_CRIMSON )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electriccrimsonLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_RED )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricredLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_ORANGE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricorangeLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_YELLOW )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricyellowLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_BROWN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricbrownLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_GREEN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricgreenLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_BLUE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricblueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_AZURE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricazureLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_CYAN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electriccyanLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}

		
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_INDIGO )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricindigoLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_PURPLE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricpurpleLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_PINK )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricpinkLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_WHITE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricwhiteLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


		else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_GREY )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricgreyLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_BLACK )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricblackLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	
	if ( fullCharge )
	{
		 if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_CRIMSON )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/crimsonLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_RED )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/redLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		  else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ORANGE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/orangeLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_YELLOW )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/yellowLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}

		  else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_BROWN )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/brownLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_GREEN )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/greenLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


		   else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_BLUE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/BlueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_AZURE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/azureLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


		    else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_CYAN )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/cyanLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_INDIGO )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/indigoLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


			 else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_PURPLE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/purpleLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_PINK )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/pinkLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


			  else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_WHITE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/whiteLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_IRIS )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/irisLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}





			   else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_GREY )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/greyLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_BLACK )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/blackLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_CRIMSON )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletriccrimsonLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_RED )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricredLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		  else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_ORANGE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricorangeLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_YELLOW )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricyellowLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}

		  else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_BROWN )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricbrownLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_GREEN )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricgreenLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


		   else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_BLUE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricBlueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_AZURE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricazureLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


		    else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_CYAN )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletriccyanLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_INDIGO )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricindigoLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


			 else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_PURPLE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricpurpleLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_PINK )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricpinkLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


			  else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_WHITE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricwhiteLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		   else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_GREY )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricgreyLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_BLACK )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricblackLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	}
}


void FX_Ray2_Alt_Shot( vec3_t start, vec3_t end, qboolean fullCharge )
{
	centity_t	*cent;
	cent = cg_entities;

	if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_CRIMSON )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/crimsonLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_RED )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/redLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ORANGE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/orangeLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_YELLOW )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/yellowLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_BROWN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/brownLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_GREEN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/greenLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_BLUE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/blueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_AZURE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/azureLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_CYAN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/cyanLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}

		
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_INDIGO )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/indigoLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_PURPLE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/purpleLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_PINK )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/pinkLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_WHITE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/whiteLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


		else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_GREY )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/greyLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_BLACK )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/blackLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_IRIS )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/irisLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_CRIMSON )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electriccrimsonLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_RED )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricredLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_ORANGE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricorangeLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_YELLOW )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricyellowLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_BROWN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricbrownLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_GREEN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricgreenLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_BLUE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricblueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_AZURE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricazureLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_CYAN )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electriccyanLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}

		
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_INDIGO )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricindigoLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_PURPLE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricpurpleLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_PINK )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricpinkLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_WHITE )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricwhiteLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


		else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_GREY )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricgreyLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_BLACK )
			{
	 			FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader("gfx/effects/electricblackLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	
	if ( fullCharge )
	{
		 if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_CRIMSON )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/crimsonLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_RED )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/redLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		  else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ORANGE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/orangeLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_YELLOW )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/yellowLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}

		  else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_BROWN )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/brownLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_GREEN )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/greenLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


		   else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_BLUE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/BlueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_AZURE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/azureLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


		    else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_CYAN )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/cyanLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_INDIGO )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/indigoLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


			 else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_PURPLE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/purpleLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_PINK )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/pinkLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


			  else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_WHITE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/whiteLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_IRIS )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/irisLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}





			   else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_GREY )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/greyLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_BLACK )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/blackLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_CRIMSON )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletriccrimsonLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_RED )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricredLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		  else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_ORANGE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricorangeLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_YELLOW )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricyellowLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}

		  else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_BROWN )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricbrownLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_GREEN )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricgreenLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


		   else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_BLUE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricBlueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_AZURE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricazureLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


		    else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_CYAN )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletriccyanLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_INDIGO )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricindigoLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


			 else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_PURPLE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricpurpleLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_PINK )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricpinkLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}


			  else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_WHITE )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricwhiteLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		   else  if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_GREY )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricgreyLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_BLACK )
			{

				vec3_t	YELLER={0.8f,0.7f,0.0f};
				// add some beef
				FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							YELLER, YELLER, 0.0f,
							150, cgi_R_RegisterShader( "gfx/effects/eletricblackLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
	}
}

void FX_Ray3_Shot( vec3_t start, vec3_t end )
{
	centity_t	*cent;
	cent = cg_entities;
	 if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_CRIMSON )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/crimsonLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_RED )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/redLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ORANGE )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/orangeLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
    else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_YELLOW )
	{
		//"concussion/beam"
		FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/yellowLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

			vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_BROWN )
		{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/brownLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_GREEN )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/greenLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}	
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_BLUE )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/blueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_AZURE )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/azureLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}	


	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_CYAN )
		{
		//"concussion/beam"
		FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/cyanLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

				vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_INDIGO )
		{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/indigoLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_PINK )
		{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/pinkLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}	
	
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_WHITE )
			{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/whiteLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	 		

	  else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_IRIS )
			{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/irisLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}


	  else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_GREY )
			
	 		{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/greyLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}

	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_BLACK )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/blackLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}

	// ELECTRIC BEAMS
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_CRIMSON )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electriccrimsonLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_RED )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricredLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_ORANGE )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricorangeLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
    else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_YELLOW )
	{
		//"concussion/beam"
		FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricyellowLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

			vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_BROWN )
		{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricbrownLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_GREEN )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricgreenLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}	
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_BLUE )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricblueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_AZURE )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricazureLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}	


	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_CYAN )
		{
		//"concussion/beam"
		FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electriccyanLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

				vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_INDIGO )
		{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricindigoLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_PINK )
		{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricpinkLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}	
	
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_WHITE )
			{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricwhiteLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
   else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_GREY )
			
	 		{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricgreyLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}

	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Ray_Effect == RAY_ELECTRIC_BLACK )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricblackLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
}


void FX_Ray3_Alt_Shot( vec3_t start, vec3_t end )
{
	centity_t	*cent;
	cent = cg_entities;
	if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_CRIMSON )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/crimsonLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_RED )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/redLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ORANGE )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/orangeLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
    else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_YELLOW )
	{
		//"concussion/beam"
		FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/yellowLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

			vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_BROWN )
		{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/brownLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_GREEN )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/greenLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}	
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_BLUE )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/blueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_AZURE )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/azureLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}	


	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_CYAN )
		{
		//"concussion/beam"
		FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/cyanLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

				vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_INDIGO )
		{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/indigoLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_PINK )
		{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/pinkLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}	
	
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_WHITE )
			{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/whiteLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	 		

	  else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_IRIS )
			{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/irisLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}


	  else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_GREY )
			
	 		{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/greyLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}

	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_BLACK )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/blackLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}

	// ELECTRIC BEAMS
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_CRIMSON )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electriccrimsonLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_RED )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricredLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_ORANGE )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricorangeLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
    else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_YELLOW )
	{
		//"concussion/beam"
		FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricyellowLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

			vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_BROWN )
		{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricbrownLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_GREEN )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricgreenLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}	
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_BLUE )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricblueLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_AZURE )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricazureLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}	


	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_CYAN )
		{
		//"concussion/beam"
		FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electriccyanLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

				vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_INDIGO )
		{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricindigoLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_PINK )
		{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricpinkLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}	
	
	 else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_WHITE )
			{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricwhiteLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
  else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_GREY )
			
	 		{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricgreyLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}

	else if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_Ray_Effect == RAY_ELECTRIC_BLACK )
	{
	//"concussion/beam"
	FX_AddLine( -1, start, end, 0.1f, 10.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							175, cgi_R_RegisterShader( "gfx/effects/electricblackLine" ), 
							0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );

	vec3_t	BRIGHT={0.75f,0.5f,1.0f};
	


	// add some beef
	FX_AddLine( -1, start, end, 0.1f, 7.0f, 0.0f, 
						1.0f, 0.0f, 0.0f,
						BRIGHT, BRIGHT, 0.0f,
						150, cgi_R_RegisterShader( "gfx/misc/whiteline2" ), 
						0, FX_SIZE_LINEAR | FX_ALPHA_LINEAR );


	}
}


void FX_Ray1_Miss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;
	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;
	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );
	centity_t	*cent;
	cent = cg_entities;
	if ( cent->gent->client && cent->gent->client->gs.gun->Gun_WallImpact_Effect )
	{		
		G_PlayEffect( cent->gent->client->gs.gun->Gun_WallImpact_Effect, origin, normal );
	}
}


void FX_Ray1_Alt_Miss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;
	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;
	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );
	centity_t	*cent;
	cent = cg_entities;
	if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_WallImpact_Effect )
	{		
		G_PlayEffect( cent->gent->client->gs.gun->Gun_Alt_WallImpact_Effect, origin, normal );
	}

}


void FX_Ray2_Miss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );

	
	centity_t	*cent;
	cent = cg_entities;
	if ( cent->gent->client && cent->gent->client->gs.gun->Gun_WallImpact_Effect )
	{		
		G_PlayEffect( cent->gent->client->gs.gun->Gun_WallImpact_Effect, origin, normal );
	}
}


void FX_Ray2_Alt_Miss( vec3_t origin, vec3_t normal )
{
	vec3_t pos, c1, c2;

	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;

	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );
	centity_t	*cent;
	cent = cg_entities;
	if ( cent->gent->client && cent->gent->client->gs.gun->Gun_Alt_WallImpact_Effect )
	{
		G_PlayEffect( cent->gent->client->gs.gun->Gun_Alt_WallImpact_Effect, origin, normal );
	}
}


void FX_Ray3_Miss( vec3_t origin, vec3_t normal )
{
	centity_t	*cent;
	cent = cg_entities;
	vec3_t pos, c1, c2;
	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;
	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );
	if  ( cent->gent->client->gs.gun->Gun_WallImpact_Effect )
	{
		G_PlayEffect( cent->gent->client->gs.gun->Gun_WallImpact_Effect, origin, normal );
	}
}


void FX_Ray3_Alt_Miss( vec3_t origin, vec3_t normal )
{
	centity_t	*cent;
	cent = cg_entities;

	vec3_t pos, c1, c2;
	VectorMA( origin, 4.0f, normal, c1 );
	VectorCopy( c1, c2 );
	c1[2] += 4;
	c2[2] += 12;
	
	VectorAdd( origin, normal, pos );
	pos[2] += 28;
	FX_AddBezier( origin, pos, c1, vec3_origin, c2, vec3_origin, 6.0f, 6.0f, 0.0f, 0.0f, 0.2f, 0.5f, WHITE, WHITE, 0.0f, 4000, cgi_R_RegisterShader( "gfx/effects/smokeTrail" ), FX_ALPHA_WAVE );
	if  ( cent->gent->client->gs.gun->Gun_Alt_WallImpact_Effect )
	{
		G_PlayEffect( cent->gent->client->gs.gun->Gun_Alt_WallImpact_Effect, origin, normal );
	}
}
