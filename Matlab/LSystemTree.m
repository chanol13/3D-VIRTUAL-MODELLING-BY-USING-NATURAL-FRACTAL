function LSystemTree
startX = 0;
startY = 0;
fm_height_max = 50; % first height max
fm_height_min = 50; % first height min
sm_height_max = 20 % second main height max
sm_height_min = 1 % second main height min
angle_max = 30; % branch angle max
angle_min = 10; % branch angle min
sb_angle_max = 30; % second branch angle max
sb_angle_min = 10; % second branch angle min
m_angle_max = 10; % main angle max
m_angle_min = 5; % main angle main
% TREE -> [FirstHeight]+[SecondHeight]
% FirstHeight -> [FHMAX-FHMIN]
% SecondHeight -> n[Branch]+[SecondHeight]//n[Branch]
% Branch -> [NBranchL]+n[NBranchL]//END
end