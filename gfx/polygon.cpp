// NCurses polygon rendering
// (c) 2022 by Stefan Roettger

#include "polygon.h"
#include "scrollarea.h"

// 2D line edge type
struct Edge2D
{
   Vec2 a, b; // line segment endpoints
   bool aa; // acute angle flag of second endpoint
   float x; // intersection coordinate
   bool f; // intersection flag
};

// calculate the intersection of a scan line with a polygon edge
void calculate_intersection(float y, Edge2D *e)
{
   // determine intersection flag
   bool f = false;
   if (y > e->a.y && y < e->b.y) f = true;
   else if (y > e->b.y && y < e->a.y) f = true;
   else if (y == e->a.y) f = true;
   else if (y == e->b.y) f = e->aa;
   e->f = f;

   // calculate intersection point
   if (f)
   {
      Vec2 d = sub2(e->b, e->a);
      float s = d.x / d.y;
      e->x = e->a.x + s*(y - e->a.y);
   }
}

// determine the ascending order of two edges
bool less(const Edge2D *e1, const Edge2D *e2)
{
   if (!e1->f) return(false);
   if (!e2->f) return(true);
   return(e1->x <= e2->x);
}

// merge two parts of the edge list
void merge(int n, int m, Edge2D e[], Edge2D t[])
{
   int p = 0;
   int p1 = 0;
   int p2 = m;

   while (p1 < m || p2 < n)
   {
      if (p1 >= m)
         t[p++] = e[p2++];
      else if (p2 >= n)
         t[p++] = e[p1++];
      else if (less(e + p1, e + p2))
         t[p++] = e[p1++];
      else
         t[p++] = e[p2++];
   }

   for (int i=0; i<n; i++)
      e[i] = t[i];
}

// use merge sort for the edge list
void merge_sort(int n, Edge2D e[], Edge2D t[])
{
   if (n > 1)
   {
      int m = n / 2;
      merge_sort(m, e, t);
      merge_sort(n - m, e + m, t);
      merge(n, m, e, t);
   }
}

// render a horizontal scan line segment
void render_scanline(float x1, float x2, int y, int ch)
{
   int i1 = x1 + 0.5;
   int i2 = x2 + 0.5;
   for (int i=i1; i<=i2; i++)
      set_cell(i, y, ch);
}

// process a scan line of a polygon with n edges
void process_scanline(int y,
                      int n, Edge2D edge[], Edge2D temp[],
                      int ch)
{
   // calculate intersection points
   for (int i=0; i<n; i++)
      calculate_intersection(y, &edge[i]);

   // sort intersections horizontally
   merge_sort(n, edge, temp);

   // process scan line segments horizontally
   int p = 0;
   float x1 = 0, x2 = 0;
   for (int i=0; i<n; i++)
   {
      // check for non-intersecting edge
      if (!edge[i].f) break;

      // count intersections
      p++;
      x1 = x2;
      x2 = edge[i].x;

      // check for intersection pair
      if (p == 2)
      {
         render_scanline(x1, x2, y, ch);
         p = 0;
      }
   }
}

// render a polygon with n vertices
void render_polygon(int n, const Vec2 vertex[],
                    int ch)
{
   if (n < 3) return;
   if (ch < 0) ch = ACS_CKBOARD;

   // transform vertex list
   Vec2 *v = new Vec2[n];
   Mat3 M = top();
   for (int i=0; i<n; i++)
   {
      Vec3 v3 = vec3(vertex[i].x, vertex[i].y);
      v[i] = mul3v(M, v3);
   }

   // determine vertical bounds
   float miny = v[0].y;
   float maxy = miny;
   for (int i=1; i<n; i++)
   {
      float y = v[i].y;
      if (y < miny) miny = y;
      if (y > maxy) maxy = y;
   }

   // determine overlapping scan lines
   int minl = miny + 0.5;
   int maxl = maxy + 0.5;

   // build edge list
   int m = 0;
   Edge2D *edge = new Edge2D[n];
   for (int i=0; i<n; i++)
   {
      // get vertices
      Vec2 a = v[i];
      Vec2 b = v[(i+1)%n];

      // discard horizontal edges from edge list
      if (a.y != b.y)
      {
         Edge2D e = {a, b, false, 0, false};
         edge[m++] = e;
      }
      else
      {
         // render horizontal edges directly
         if (a.x < b.x)
            render_scanline(a.x, b.x, a.y + 0.5, ch);
         else
            render_scanline(b.x, a.x, a.y + 0.5, ch);
      }
   }

   // process edge list
   for (int i=0; i<m; i++)
   {
      // get vertices
      Vec2 a = edge[i].a;
      Vec2 b = edge[i].b;
      Vec2 c = edge[(i+1)%m].b;

      // determine acute angle flag
      edge[i].aa = (b.y - a.y) * (c.y - b.y) < 0;
   }

   // process scan lines
   Edge2D *temp = new Edge2D[m];
   for (int l=minl; l<=maxl; l++)
      process_scanline(l, m, edge, temp, ch);

   // release edge lists
   delete[] edge;
   delete[] temp;
   delete[] v;
}

// render a triangle with 3 vertices
void render_triangle(const Vec2 &v1, const Vec2 &v2, const Vec2 &v3,
                     int ch)
{
   Vec2 v[] = {v1, v2, v3};
   render_polygon(3, v, ch);
}
