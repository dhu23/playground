% A standard modeling techique is to divide the life cycle of a species
% into a number of stages. 
% The models assume that 
%    - the population sizes for each stage depend only on the female population
%    - the probability of survival of an individual female from one year to the
%      next depends only on the stage of the life cycle and not on the actual
%      age of an individual


% ---------------------------------------------------------------------
% for a 4-stage model for Loggerhead Sea Turtle demographics
% 
% s = annual survivalship
% e = egg laid per year
% stage 1 (< 1 year): eggs, hatchings
s1 = 0.67; 
e1 = 0;
% stage 2 (1-21 years): juveniles and subadults
s2 = 0.74;
e2 = 0;
% stage 3 (22nd year): novice breeders
s3 = 0.81;
e3 = 127;
% stage 4 (23 - 54 years): mature breeders
s4 = 0.81; 
e4 = 79;

% analysis and solution

% let ai = a0, a1, a2, ... be the population for different ages this year
% let bi = b0, b1, b2, ... be the population for different ages next eyar

% we can derive based on the model. 
% b0 = a22 * e3 + (a23 + a24 + ...) * e4
% b1 = a0 * s1
% b2 = a1 * s2
% ...
% b21 = a20 * s2
% b22 = a21 * s2
% b23 = a22 * s3
% b24 = a23 * s4
% ...

% Given that there is no "immigration" at each age > 1, within a stage, 
% population should form a geometric sequence

% let A1, A2, A3, A4 be the population for the 4 stages this year
% let B1, B2, B3, B4 be the population for the 4 stages next eyar

% A1 = a0
% A2 = a1 + ... + a21
% A3 = a22
% A4 = a23 + ... + a54 + ...

%%%
% B1 = b0 = A3 * e3 + A4 * e4

%%%
% B2 = b1 + b2 + ... b21 = A1 * s1 + (a1 + ... + a20) * s2

% (a1 + ... + a20) / A2 = (a1 + .. + a20) / (a1 + ... + a21)
%                       = (1-(s2)^19) / (1-(s2)^20)
% let's call this portion of stage at stage 2, P2
% so B2 = A1 * s1 + A2 * P2 * s2

% B3 = (1-P2) * A2 * s2

% B4 = A4 * s4 + A3 * s3

P2 = (1 - s2^19) / (1- s2^20);

% one tweak we can make: here we are considering population of stage 4
% stay in that stage forever but we should consider a stage 5 that doesn't
% show up in the table which lays 0 eggs, in that case B4 needs to be 
% revised slightly for A4's coefficient
T = [0 0 e3 e4; s1 P2*s2 0 0; 0 (1-P2)*s2 0 0; 0 0 s3 s4]
