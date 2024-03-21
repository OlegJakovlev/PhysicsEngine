#pragma once

#ifndef JointType_H
#define JointType_H

namespace PhysicsEngine
{
	enum JointType
	{
		Fixed,
		Distance,
		Spherical,
		Revolute,
		Prismatic,
		Gear, // Not available in physx 3.4
		RackAndPinion, // Not available in physx 3.4
		D6,
	};
}

#endif