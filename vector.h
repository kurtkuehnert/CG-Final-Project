#ifndef VECTOR_H
#define VECTOR_H

class CVector {
	private:
		float _v[3];

	public:
		// Konstruktoren
		CVector();
		CVector(float x, float y, float z);
		CVector(const float *);

		// Setter
		void set(const float *a);
		void set(float x, float y, float z);
		void zero();

		// Getter
		float *get_floats();
		float x() const;
		float y() const;
		float z() const;
		float len() const;
		float len_sqr() const;

		// Operatoren
		CVector operator + (const CVector& b) const;
		CVector operator - (const CVector& b) const;
		CVector operator * (const float s) const;		/* Vektor * Skalar */
		CVector operator * (const CVector& b) const;	/* Kreuzprodukt */
		float dot3(const CVector& b) const;			/* Punktprodukt */

		CVector rotateY(const float rot) const;	/* liefert einen um Y-Achse rotierten Vektor*/

		// Normalisieren
		void normalize();

		// Debug-Print
		void print(const char *prefix="") const;
};


#endif /* VECTOR_H */
