load('\\netapp2.iitd.ernet.in\homes\mech\btech\me2120861\Windows\Desktop\kmeans10_1194_data.mat')
load('\\netapp2.iitd.ernet.in\homes\mech\btech\me2120861\Windows\Desktop\2012ME20861.mat')
%sigma = zeros(10,10)

X = data_image;
T = data_labels;
X1 = double(X);
mu = [];
sig = cell(1,10);

for i=1:10
    mui = C(i,:);
    mu = [mu;mui];
    temp = X1(idx==i,:);
    sigmai = cov(temp);
    sig{i} = sigmai; 
end

%[mu,s,muci,sci] = normfit(height)

pi = (1/10)*ones(10,1);

loops = 0;
gamma = zeros(size(X1,1),10);
Nk = zeros(1,10);
oldmu = mu;
while loops<50
    % expectation step
    for i = 1:size(X1,1)
        sum = 0
        for k = 1:10
            gamma(i,k) = pi(k)*mvnpdf(X1(i,:),mu(k,:),sig{k});
            sum = sum + gamma(i,k); 
        end
        for k = 1:10
            gamma (i,k) = gamma (i,k)/sum;
        end
    end
    % maximization step
    Nk = sum(gamma);                
    %mu
    mu = (gamma' * X1)./(Nk'*ones(1,size(X1,2)));
    %sigma
    for k=1:10
        sigmak = 0;
        for i=1:size(X1,1)
            sigmak =  sigmak + gamma(i,k).*((X1(i,:) - mu(k,:))'*((X1(i,:) - mu(k,:)))) ;
        end
        sigmak = sigmak ./ Nk(k);
        sig{k} = sigmak;
    end
    pi = Nk'./size(X1,1);
    if (sum(abs(mu-oldmu)) < 0.05)
        break
    end
end

    
 