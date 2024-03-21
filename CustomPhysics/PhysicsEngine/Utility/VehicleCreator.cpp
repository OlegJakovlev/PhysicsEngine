#include "VehicleCreator.h"

namespace PhysicsEngine
{
	bool VehicleCreator::Init(physx::PxPhysics* physics)
	{
        m_physicsRef = physics;
		return true;
	}

	void VehicleCreator::SetupWheelsSimulationData(const VehicleData* configData)
	{
        auto wheelData = configData->wheelData;
        uint32_t totalWheels = configData->drivableWheels + configData->undrivableWheels;

        physx::PxVehicleWheelData wheels[PX_MAX_NB_WHEELS];
        {
            //Set up the wheel data structures with mass, moi, radius, width.
            for (physx::PxU32 i = 0; i < totalWheels; i++)
            {
                wheels[i].mMass = wheelData->wheelMass;
                wheels[i].mMOI = wheelData->wheelMOI;
                wheels[i].mRadius = wheelData->wheelRadius;
                wheels[i].mWidth = wheelData->wheelWidth;
            }

            //Enable the handbrake for the rear wheels only.
            wheels[physx::PxVehicleDrive4WWheelOrder::eREAR_LEFT].mMaxHandBrakeTorque = 4000.0f;
            wheels[physx::PxVehicleDrive4WWheelOrder::eREAR_RIGHT].mMaxHandBrakeTorque = 4000.0f;

            //Enable steering for the front wheels only.
            wheels[physx::PxVehicleDrive4WWheelOrder::eFRONT_LEFT].mMaxSteer = physx::PxPi * 0.3333f;
            wheels[physx::PxVehicleDrive4WWheelOrder::eFRONT_RIGHT].mMaxSteer = physx::PxPi * 0.3333f;
        }

        //Set up the tires.
        physx::PxVehicleTireData tires[PX_MAX_NB_WHEELS];
        {
            //Set up the tires.
            for (physx::PxU32 i = 0; i < totalWheels; i++)
            {
                //tires[i].mType = TIRE_TYPE_NORMAL;
            }
        }

        //Set up the suspensions
        physx::PxVehicleSuspensionData suspensions[PX_MAX_NB_WHEELS];
        {
            //Compute the mass supported by each suspension spring.
            physx::PxF32 suspSprungMasses[PX_MAX_NB_WHEELS];
            physx::PxVehicleComputeSprungMasses(totalWheels, wheelData->wheelCenterActorOffsets, wheelData->chassisCMOffset, wheelData->chassisMass, 1, suspSprungMasses);

            //Set the suspension data.
            for (physx::PxU32 i = 0; i < totalWheels; i++)
            {
                suspensions[i].mMaxCompression = 0.3f;
                suspensions[i].mMaxDroop = 0.1f;
                suspensions[i].mSpringStrength = 35000.0f;
                suspensions[i].mSpringDamperRate = 4500.0f;
                suspensions[i].mSprungMass = suspSprungMasses[i];
            }

            //Set the camber angles.
            const physx::PxF32 camberAngleAtRest = 0.0;
            const physx::PxF32 camberAngleAtMaxDroop = 0.01f;
            const physx::PxF32 camberAngleAtMaxCompression = -0.01f;

            for (physx::PxU32 i = 0; i < totalWheels; i += 2)
            {
                suspensions[i + 0].mCamberAtRest = camberAngleAtRest;
                suspensions[i + 1].mCamberAtRest = -camberAngleAtRest;
                suspensions[i + 0].mCamberAtMaxDroop = camberAngleAtMaxDroop;
                suspensions[i + 1].mCamberAtMaxDroop = -camberAngleAtMaxDroop;
                suspensions[i + 0].mCamberAtMaxCompression = camberAngleAtMaxCompression;
                suspensions[i + 1].mCamberAtMaxCompression = -camberAngleAtMaxCompression;
            }
        }

        //Set up the wheel geometry.
        physx::PxVec3 suspTravelDirections[PX_MAX_NB_WHEELS];
        physx::PxVec3 wheelCentreCMOffsets[PX_MAX_NB_WHEELS];
        physx::PxVec3 suspForceAppCMOffsets[PX_MAX_NB_WHEELS];
        physx::PxVec3 tireForceAppCMOffsets[PX_MAX_NB_WHEELS];
        {
            //Set the geometry data.
            for (physx::PxU32 i = 0; i < totalWheels; i++)
            {
                //Vertical suspension travel.
                suspTravelDirections[i] = physx::PxVec3(0, -1, 0);

                //Wheel center offset is offset from rigid body center of mass.
                wheelCentreCMOffsets[i] = wheelData->wheelCenterActorOffsets[i] - wheelData->chassisCMOffset;

                //Suspension force application point 0.3 metres below
                //rigid body center of mass.
                suspForceAppCMOffsets[i] = physx::PxVec3(wheelCentreCMOffsets[i].x, -0.3f, wheelCentreCMOffsets[i].z);

                //Tire force application point 0.3 metres below
                //rigid body center of mass.
                tireForceAppCMOffsets[i] = physx::PxVec3(wheelCentreCMOffsets[i].x, -0.3f, wheelCentreCMOffsets[i].z);
            }
        }

        //Set up the filter data of the raycast that will be issued by each suspension.
        //physx::PxFilterData qryFilterData;
        //setupNonDrivableSurface(qryFilterData);

        //Set the wheel, tire and suspension data.
        //Set the geometry data.
        //Set the query filter data
        auto wheelSimData = configData->wheelSimData;

        for (physx::PxU32 i = 0; i < totalWheels; i++)
        {
            wheelSimData->setWheelData(i, wheels[i]);
            wheelSimData->setTireData(i, tires[i]);
            wheelSimData->setSuspensionData(i, suspensions[i]);
            wheelSimData->setSuspTravelDirection(i, suspTravelDirections[i]);
            wheelSimData->setWheelCentreOffset(i, wheelCentreCMOffsets[i]);
            wheelSimData->setSuspForceAppPointOffset(i, suspForceAppCMOffsets[i]);
            wheelSimData->setTireForceAppPointOffset(i, tireForceAppCMOffsets[i]);
            //wheelSimData->setSceneQueryFilterData(i, qryFilterData);
            wheelSimData->setWheelShapeMapping(i, i);
        }
	}

    void SetupDriveSimulation4W(const VehicleData* configData)
    {
        auto* drive4WSimData = (physx::PxVehicleDriveSimData4W*) configData->driveSimData;
        auto* wheelsSimData = configData->wheelSimData;

        // Diff
        physx::PxVehicleDifferential4WData diff4W;
        diff4W.mType = physx::PxVehicleDifferential4WData::eDIFF_TYPE_LS_4WD;
        drive4WSimData->setDiffData(diff4W);

        // Engine
        physx::PxVehicleEngineData engine;
        engine.mPeakTorque = 500.0f;
        engine.mMaxOmega = 600.0f;
        drive4WSimData->setEngineData(engine);

        // Gears
        physx::PxVehicleGearsData gears;
        gears.mSwitchTime = 0.5f;
        drive4WSimData->setGearsData(gears);

        // Clutch
        physx::PxVehicleClutchData clutch;
        clutch.mStrength = 10.0f;
        drive4WSimData->setClutchData(clutch);

        // Ackermann steer accuracy
        physx::PxVehicleAckermannGeometryData ackermann;
        ackermann.mAccuracy = 1.0f;
        ackermann.mAxleSeparation = wheelsSimData->getWheelCentreOffset(physx::PxVehicleDrive4WWheelOrder::eFRONT_LEFT).z
                                    - wheelsSimData->getWheelCentreOffset(physx::PxVehicleDrive4WWheelOrder::eREAR_LEFT).z;

        ackermann.mFrontWidth = wheelsSimData->getWheelCentreOffset(physx::PxVehicleDrive4WWheelOrder::eFRONT_RIGHT).x
                                - wheelsSimData->getWheelCentreOffset(physx::PxVehicleDrive4WWheelOrder::eFRONT_LEFT).x;

        ackermann.mRearWidth = wheelsSimData->getWheelCentreOffset(physx::PxVehicleDrive4WWheelOrder::eREAR_RIGHT).x
                                - wheelsSimData->getWheelCentreOffset(physx::PxVehicleDrive4WWheelOrder::eREAR_LEFT).x;

        drive4WSimData->setAckermannGeometryData(ackermann);
    }

    void SetupDriveSimulationTank(const VehicleData* configData)
    {
    }

    void SetupDriveSimulationNW(const VehicleData* configData)
    {
        physx::PxVehicleDriveSimDataNW* driveNWSimData = (physx::PxVehicleDriveSimDataNW*) configData->driveSimData;

        // Diff
        physx::PxVehicleDifferentialNWData diffNW;
        driveNWSimData->setDiffData(diffNW);

        // Engine
        physx::PxVehicleEngineData engine;
        engine.mPeakTorque = 500.0f;
        engine.mMaxOmega = 600.0f;
        driveNWSimData->setEngineData(engine);

        // Gears
        physx::PxVehicleGearsData gears;
        gears.mSwitchTime = 0.5f;
        driveNWSimData->setGearsData(gears);

        // Clutch
        physx::PxVehicleClutchData clutch;
        clutch.mStrength = 10.0f;
        driveNWSimData->setClutchData(clutch);

        // Autobox
        physx::PxVehicleAutoBoxData autoBox;
        driveNWSimData->setAutoBoxData(autoBox);
    }

	void VehicleCreator::SetupDriveSimulationData(const VehicleData* configData)
	{
        switch (configData->type)
        {
            case VehicleType::Default4W:
                SetupDriveSimulation4W(configData);
                break;

            case VehicleType::Tank:
                SetupDriveSimulationTank(configData);
                break;

            case VehicleType::Custom:
                SetupDriveSimulationNW(configData);
                break;
        }
	}

	void VehicleCreator::SetupVehicleActor(physx::PxRigidDynamic* vehActor, const VehicleData* configData)
	{
        //Wheel and chassis query filter data.
        //Optional: cars don't drive on other cars.
        //physx::PxFilterData wheelQryFilterData;
        //setupNonDrivableSurface(wheelQryFilterData);

        //physx::PxFilterData chassisQryFilterData;
        //setupNonDrivableSurface(chassisQryFilterData);

        uint32_t numOfWheels = configData->drivableWheels + configData->undrivableWheels;

        // Add all the wheel shapes to the actor.
        /* TODO
        for (physx::PxU32 i = 0; i < numOfWheels; i++)
        {
            physx::PxShape* wheelShape = physx::PxRigidActorExt::createExclusiveShape(*vehActor, *wheelGeometries[i], *wheelMaterial);
            wheelShape->setQueryFilterData(vehQryFilterData);
            wheelShape->setSimulationFilterData(wheelCollFilterData);
            wheelShape->setLocalPose(physx::PxTransform(physx::PxIdentity));
        }
        */

        auto actorData = configData->actorData;

        // Add the chassis shapes to the actor.
        /* TODO
        for (physx::PxU32 i = 0; i < actorData->numChassisMeshes; i++)
        {
            physx::PxShape* chassisShape = physx::PxRigidActorExt::createExclusiveShape(*vehActor, *chassisGeometries[i], *chassisMaterial);
            chassisShape->setQueryFilterData(vehQryFilterData);
            chassisShape->setSimulationFilterData(chassisCollFilterData);
            chassisShape->setLocalPose(chassisLocalPoses[i]);
        }
        */

        vehActor->setMass(actorData->chassisData.mMass);
        vehActor->setMassSpaceInertiaTensor(actorData->chassisData.mMOI);
        vehActor->setCMassLocalPose(physx::PxTransform(actorData->chassisData.mCMOffset, physx::PxQuat(physx::PxIdentity)));
	}

	physx::PxVehicleDrive* VehicleCreator::Create4W(physx::PxRigidDynamic* rigidBodyActor, const VehicleData* configData)
	{
		int undrivableWheels = configData->undrivableWheels;

		auto* nonCastVehicleSimData = const_cast<physx::PxVehicleDriveSimData*>(configData->driveSimData);
		auto* vehicleDriveSimData4W = (physx::PxVehicleDriveSimData4W*) nonCastVehicleSimData;

		physx::PxVehicleDrive4W* vehicleDrive4W = physx::PxVehicleDrive4W::allocate(configData->drivableWheels + undrivableWheels);
		vehicleDrive4W->setup(m_physicsRef, rigidBodyActor, *configData->wheelSimData, *vehicleDriveSimData4W, undrivableWheels);

		return vehicleDrive4W;
	}

	physx::PxVehicleDrive* VehicleCreator::CreateNW(physx::PxRigidDynamic* rigidBodyActor, const VehicleData* configData)
	{
		int undrivableWheels = configData->undrivableWheels;

		auto* nonConstVehicleSimData = const_cast<physx::PxVehicleDriveSimData*>(configData->driveSimData);
		auto* vehicleDriveSimDataNW = (physx::PxVehicleDriveSimDataNW*) nonConstVehicleSimData;

		physx::PxVehicleDriveNW* vehDriveNW = physx::PxVehicleDriveNW::allocate(configData->drivableWheels + undrivableWheels);
		vehDriveNW->setup(m_physicsRef, rigidBodyActor, *configData->wheelSimData, *vehicleDriveSimDataNW, undrivableWheels);

		return vehDriveNW;
	}

	physx::PxVehicleDrive* VehicleCreator::CreateTank(physx::PxRigidDynamic* rigidBodyActor, const VehicleData* configData)
	{
		int undrivableWheels = configData->undrivableWheels;

		auto* nonConstVehicleSimData = const_cast<physx::PxVehicleDriveSimData*>(configData->driveSimData);
		auto* vehicleDriveSimDataNW = (physx::PxVehicleDriveSimDataNW*) nonConstVehicleSimData;

		physx::PxVehicleDriveTank* vehDriveTank = physx::PxVehicleDriveTank::allocate(configData->drivableWheels + undrivableWheels);
		vehDriveTank->setup(m_physicsRef, rigidBodyActor, *configData->wheelSimData, *vehicleDriveSimDataNW, undrivableWheels);

		return vehDriveTank;
	}
}
