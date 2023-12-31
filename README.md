# RobotArmIKExam

<p align="center">
  <img src="https://github.com/jellypower/PublicImageDataBase/blob/main/Portfolio/Blogs/RobotArm.gif" width="300" />
</p>

## 프로젝트의 목표
- IK작동방식과 그 이론을 학습하고 OpenGL을 통해 구현하는 학습용 프로젝트



## IK기능은 무엇인가?

- 보통 골격구조를 가진 캐릭터의 움직임에서, 특정 관절의 “위치”는 중심으로부터 뻗어나오는 해당 캐릭터의 관절의 “각도”로부터 파생됩니다.
- 그렇기에 “각도”를 통해 “위치”를 구하는 것은 골격구조상으로 자연스럽게 일어나고 당연한 일입니다.
- 그러나, 내가 원하는 “위치”에 캐릭터의 특정 골격을 위치시키고 싶은 경우(탁자 위에 손을 올려놓는 상황을 연출하고 싶거나 하는 경우)에는 특별한 로직이 필요합니다.
- 위처럼 “위치”기반으로 관절의 “각도”를 구하는 방식을 보통 IK(Inverse Kinematics)라고 부르며 이를 지지하는 기반 이론은 머신러닝과 맞닿아 있습니다.

- 자세한 설명은 아래 링크를 참조해주세요

## Link

[IK는 내부적으로 어떻게 작동하는가?](https://velog.io/@jellypower/%EC%BA%90%EB%A6%AD%ED%84%B0-%EC%95%A0%EB%8B%88%EB%A9%94%EC%9D%B4%EC%85%98%EA%B3%BC-%EB%A8%B8%EC%8B%A0%EB%9F%AC%EB%8B%9DIK%EB%8A%94-%EB%82%B4%EB%B6%80%EC%A0%81%EC%9C%BC%EB%A1%9C-%EC%96%B4%EB%96%BB%EA%B2%8C-%EC%9E%91%EB%8F%99%ED%95%98%EB%8A%94%EA%B0%80)
