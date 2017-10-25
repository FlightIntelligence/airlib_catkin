// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_AirSimSimpleFlightEstimator_hpp
#define msr_airlib_AirSimSimpleFlightEstimator_hpp

#include "firmware/interfaces/CommonStructs.hpp"
#include "AirSimSimpleFlightCommon.hpp"
#include "physics/Kinematics.hpp"
#include "physics/Environment.hpp"
#include "common/Common.hpp"

namespace msr { namespace airlib {


class AirSimSimpleFlightEstimator : public simple_flight::IStateEstimator {
public:
    //for now we don't do any state estimation and use ground truth (i.e. assume perfect sensors)
    void setKinematics(const Kinematics::State* kinematics, Environment* environment)
    {
        kinematics_ = kinematics;
        environment_ = environment;
    }

    virtual simple_flight::Axis3r getAngles() const override
    {
        simple_flight::Axis3r angles;
        VectorMath::toEulerianAngle(kinematics_->pose.orientation,
            angles.pitch(), angles.roll(), angles.yaw());

        //Utils::log(Utils::stringf("Ang Est:\t(%f, %f, %f)", angles.pitch(), angles.roll(), angles.yaw()));

        return angles;
    }

    virtual simple_flight::Axis3r getAngulerVelocity() const override
    {
        const auto& anguler = kinematics_->twist.angular;

        simple_flight::Axis3r conv;
        conv.x() = anguler.x(); conv.y() = anguler.y(); conv.z() = anguler.z();

        return conv;
    }

    virtual simple_flight::Axis3r getPosition() const override
    {
        return AirSimSimpleFlightCommon::toAxis3r(kinematics_->pose.position);
    }

    virtual simple_flight::Axis3r transformToBodyFrame(const simple_flight::Axis3r& world_frame_val) const override
    {
        const Vector3r& vec = AirSimSimpleFlightCommon::toVector3r(world_frame_val);
        const Vector3r& trans = VectorMath::transformToBodyFrame(vec, kinematics_->pose.orientation);
        return AirSimSimpleFlightCommon::toAxis3r(trans);
    }

    virtual simple_flight::Axis3r getLinearVelocity() const override
    {
        return AirSimSimpleFlightCommon::toAxis3r(kinematics_->twist.linear);
    }

    virtual simple_flight::Axis4r getOrientation() const override
    {
        return AirSimSimpleFlightCommon::toAxis4r(kinematics_->pose.orientation);
    }

    virtual simple_flight::GeoPoint getGeoPoint() const override
    {
        return AirSimSimpleFlightCommon::toSimpleFlightGeoPoint(environment_->getState().geo_point);
    }

    virtual void setHomeGeoPoint(const simple_flight::GeoPoint& geo_point) override
    {
        environment_->setHomeGeoPoint(AirSimSimpleFlightCommon::toGeoPoint(geo_point));
    }

    virtual simple_flight::GeoPoint getHomeGeoPoint() const override
    {
        return AirSimSimpleFlightCommon::toSimpleFlightGeoPoint(environment_->getHomeGeoPoint());
    }

private:
    const Kinematics::State* kinematics_;
    Environment* environment_;
};


}} //namespace
#endif
