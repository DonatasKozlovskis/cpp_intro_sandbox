#include <iostream>
#include <iomanip> // pour setfill
#include <string>
#include <vector>
using namespace std;

typedef string Day; // type pour définir les jous

void print_time(double); // fonction utile pour afficher les heures

class Time
/* Représente le jour et l'heure d'un évènement.
 * Les heures sont représentées en double depuis minuit.
 * Par exemple 14:30 est représenté 14.5.
 * La fonction-outil print_time permet d'afficher de telles heures.
 */
{
public:
  // Constructeur à partir du jour et de l'heure
  Time(Day jour, double heure)
    : day_(jour), hour_(heure) {
  }

  // Affichage
  void print() const {
    cout << day_ << " à ";
    print_time(hour_);
  }

  // Pour connaître le jour
  Day day() const {
    return day_;
  }

  // Pour connaître l'heure
  double hour() const {
    return hour_;
  }

private:
  Day day_;
  double hour_;
};

/* Fonction outil pour afficher les temps représentés en double
 * comme indiqué ci-dessus.
 */
void print_time(double t)
{
  cout << setfill('0') << setw(2) << int(t)
       << setfill('0') << setw(1) << ":"
       << setfill('0') << setw(2) << int(60.0 * (t - int(t)));
}

/* Type utilisé pour identifier les cours.
 * Chaque cours aura en effet un identificateur unique ; par exemple "CS-101".
 */
typedef string CourseId;

/*******************************************
 * Complétez le programme à partir d'ici.
 *******************************************/

class Activity
/*
*/
{
public:
	// Constructeur
	Activity(string location, Day day, double hour, double duration)
		: location_(location), duration_(duration), startTime_(day, hour) {
	}
	//delete copy constructor
	Activity(const Activity&) = delete;
	
	// Getters
	string getLocation() const {
		return location_;
	}
	Time getTime() const {
		return startTime_;
	}
	double getDuration() const {
		return duration_;
	}
	// methods
	bool conflicts(const Activity& that) const {
		bool isConflict = false;
		
		if (this->startTime_.day() == that.startTime_.day()) {
			if (this->startTime_.hour() <= that.startTime_.hour() &&
				(that.startTime_.hour() < this->startTime_.hour() + this->duration_)) {

				isConflict = true;
			}
			if (this->startTime_.hour() < ( that.startTime_.hour() + that.duration_) &&
				((that.startTime_.hour() + that.duration_) <= this->startTime_.hour() + this->duration_)) {

				isConflict = true;
			}
			if (this->startTime_.hour() >= that.startTime_.hour() &&
				this->startTime_.hour() < (that.startTime_.hour() + that.duration_) ) {
				isConflict = true;
			}
			if ((this->startTime_.hour() + this->duration_) > that.startTime_.hour() &&
				(this->startTime_.hour() + this->duration_) <= (that.startTime_.hour() + that.duration_)) {
				isConflict = true;
			}
			if ( this->startTime_.hour() == that.startTime_.hour() ) {
				isConflict = true;
			}
		}
	
		return isConflict;
	}
	void print() const {
		cout << "le "; 
		startTime_.print();

		cout << " en " << location_ << ", durée ";
		print_time(duration_);
	}
private:
	string location_;
	double duration_;
	Time startTime_;
};

class Course
/*
*/
{
public:
	// Constructor
	Course(CourseId id, string name, const Activity& lecture, const Activity& exerciseSession, int credits)
		: id_(id), name_(name), 
		activityLecture_(lecture.getLocation(), lecture.getTime().day(), lecture.getTime().hour(), lecture.getDuration() ),
		activityExercise_(exerciseSession.getLocation(), exerciseSession.getTime().day(), exerciseSession.getTime().hour(), exerciseSession.getDuration()),
		credits_(credits) 
	{
		cout << "Nouveau cours : " << id_ << endl;
	}
	// Delete copy constructor
	Course(const Course&) = delete;

	// Destructor
	~Course() {
		cout << "Suppression du cours : " << id_ << endl;
	}
	// Getters
	CourseId getId() const {
		return id_;
	}
	string getTitle() const {
		return name_;
	}
	int getCredits() const {
		return credits_;
	}
	double workload() const {
		return activityLecture_.getDuration() + activityExercise_.getDuration();
	}
	bool conflicts(const Activity& that) const {
		bool isConflict = false;
		isConflict = activityLecture_.conflicts(that) || activityExercise_.conflicts(that);

		return isConflict;
	}
	bool conflicts(const Course& that) const {
		bool isConflict = false;
		
		isConflict = that.conflicts(activityLecture_) || that.conflicts(activityExercise_);

		return isConflict;
	}

	void print() const {
		cout << id_ << ": " << name_ << " - cours ";
		activityLecture_.print();
		cout << ", exercices ";
		activityExercise_.print();
		cout << ". crédits : " << credits_;
	}

private:
	CourseId id_;
	string name_;
	Activity activityLecture_;
	Activity activityExercise_;
	int credits_;
};

class StudyPlan
/*

*/
{
public:
	void add_course( const Course& newCourse ) {
		courses_.push_back( &newCourse );
	}

	bool conflicts(CourseId id, vector<CourseId> otherIds) const {
		size_t indexCourse, indexOther;
		if (findCourse(id, indexCourse)) {
			for (size_t i = 0; i != otherIds.size(); i++) {
				if (findCourse(otherIds[i], indexOther)) {
					if (courses_[indexCourse]->conflicts(*courses_[indexOther])) {
						return true;
					}
				}
			}
		}
		return false;
	}

	int credits(CourseId id) const {
		size_t index;
		if (findCourse(id, index)) {
			return courses_[index]->getCredits();
		}
		return 0;
	}
	double workload(CourseId id) const {
		size_t index;
		if (findCourse(id, index)) {
			return courses_[index]->workload();
		}
		return 0;
	}
	void print(CourseId id) const {
		size_t index;
		if (findCourse(id, index))
			courses_[index]->print();
	}
	void printCourseSuggestions(vector<CourseId> courseIds) const {
		bool isConflict;
		bool allConflicts = true;
		for (size_t i = 0; i != courses_.size(); i++) {

			isConflict = conflicts(courses_[i]->getId(), courseIds);
			
			allConflicts = allConflicts && isConflict;

			if (!isConflict) {
				courses_[i]->print();
				cout << endl;
			}
		}

		if (allConflicts) {
			cout << "Aucun cours n'est compatible avec la sélection de cours.";
		}
	}

private:
	vector<const Course*> courses_;

	bool findCourse(CourseId id, size_t& index) const {
		for (size_t i = 0; i != courses_.size(); i++) {
			if (id == courses_[i]->getId()) {
				index = i;
				return true;
			}
		}
		return false;
	}

};

class Schedule
/*

*/
{
public:
	// Constructeur
	Schedule(const StudyPlan& studyPlan) {
		studyPlan_ = &studyPlan;
	}

	bool add_course(CourseId id) {
		// if no conflicts, add course
		if (!studyPlan_->conflicts(id, courseIds_)) {
			courseIds_.push_back(id);
			return true;
		}
		// otherwise return false
		return false;
	}

	double computeDailyWorkload() const {
		double totalWorkload  = 0 ;
		int numberOfStudyDays = 5;
		for (size_t i = 0; i != courseIds_.size(); i++) {
			totalWorkload += studyPlan_->workload(courseIds_[i]);
		}

		return totalWorkload / numberOfStudyDays;
	}

	int computeTotalCredits() const {
		int totalCredits = 0;
		for (size_t i = 0; i != courseIds_.size(); i++) {
			totalCredits += studyPlan_->credits(courseIds_[i]);
		}
		return totalCredits;
	}

	void print() const {
		for (size_t i = 0; i != courseIds_.size(); i++) {
			studyPlan_->print( courseIds_[i] );
			cout << endl;
		}
		cout << "Total de crédits   : " << computeTotalCredits() << endl;
		cout << "Charge journalière : ";
		print_time(computeDailyWorkload());
		cout << endl;
		cout << "Suggestions :" << endl;
		studyPlan_->printCourseSuggestions(courseIds_);
	}


private:
	vector<CourseId> courseIds_;
	const StudyPlan * studyPlan_;
};
/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/

int main()
{
  // Quelques activités
  Activity physicsLecture  ("Central Hall", "lundi",  9.25, 1.75);
  Activity physicsExercises("Central 101" , "lundi", 14.00, 2.00);

  Activity historyLecture  ("North Hall", "lundi", 10.25, 1.75);
  Activity historyExercises("East 201"  , "mardi",  9.00, 2.00);

  Activity financeLecture  ("South Hall",  "vendredi", 14.00, 2.00);
  Activity financeExercises("Central 105", "vendredi", 16.00, 1.00);

  // On affiche quelques informations sur ces activités
  cout << "L'activité physicsLecture a lieu ";
  physicsLecture.print();
  cout << "." << endl;

  cout << "L'activité historyLecture a lieu ";
  historyLecture.print();
  cout << "." << endl;

  if (physicsLecture.conflicts(historyLecture)) {
    cout << "physicsLecture est en conflit avec historyLecture."
         << endl;
  } else {
    cout << "physicsLecture n'est pas en conflit avec historyLecture."
         << endl;
  }
  cout << endl;

  // Création d'un plan d'étude
  StudyPlan studyPlan;
  Course physics("PHY-101", "Physique", physicsLecture, physicsExercises, 4);
  studyPlan.add_course(physics);
  Course history("HIS-101", "Histoire", historyLecture, historyExercises, 4);
  studyPlan.add_course(history);
  Course finance("ECN-214", "Finance" , financeLecture, financeExercises, 3);
  studyPlan.add_course(finance);

  // Première tentative d'emploi du temps
  Schedule schedule1(studyPlan);
  schedule1.add_course(finance.getId());
  cout << "Emploi du temps 1 :" << endl;
  schedule1.print();

  /* On ne sait pas encore très bien quoi faire : on essaye donc
   * sur une copie de l'emploi du temps précédent.
   */
  Schedule schedule2(schedule1);
  schedule2.add_course(history.getId());
  cout << "Emploi du temps 2 :" << endl;
  schedule2.print();

  // Un troisième essai
  Schedule schedule3(studyPlan);
  schedule3.add_course(physics.getId());
  cout << "Emploi du temps 3 :" << endl;
  schedule3.print();

  return 0;
}
