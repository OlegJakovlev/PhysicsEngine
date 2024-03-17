#include "GlutRenderer.h"
#include <GL/glut.h>
#include <vector>
#include "../../GameObjects/GameObject.h"
#include "../../../PhysicsEngine/Engine/Actors/Actor.h"
#include <string>
#include "../../GameObjects/ClothGameObject.h"

namespace CustomApplication
{
	void GlutRenderer::SetupWindow(const char* name, int width, int height)
	{
		char* namestr = new char[strlen(name) + 1];
		strcpy_s(namestr, strlen(name) + 1, name);
		int argc = 1;
		char* argv[1] = { namestr };

		glutInit(&argc, argv);

		glutInitWindowSize(width, height);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		glutSetWindow(glutCreateWindow(name));

		delete[] namestr;
	}

	void GlutRenderer::SetupRenderStates()
	{
		physx::PxReal specular_material[] = { .1f, .1f, .1f, 1.f };
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 1.f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material);
	}

	void GlutRenderer::SetupLighting()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glEnable(GL_LIGHTING);
		physx::PxReal ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.f };
		physx::PxReal diffuseColor[] = { 0.7f, 0.7f, 0.7f, 1.f };
		physx::PxReal position[] = { 50.f, 50.f, 100.f, 0.f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glEnable(GL_LIGHT0);
	}

	void GlutRenderer::DrawPlane() const
	{
		glScalef(10240, 0, 10240);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glVertexPointer(3, GL_FLOAT, 2 * 3 * sizeof(float), k_PlaneData);
		glNormalPointer(GL_FLOAT, 2 * 3 * sizeof(float), k_PlaneData + 3);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	void GlutRenderer::DrawSphere(const physx::PxGeometryHolder& geometry) const
	{
		glutSolidSphere(geometry.sphere().radius, m_renderDetail, m_renderDetail);
	}

	void GlutRenderer::DrawBox(const physx::PxGeometryHolder& geometry) const
	{
		physx::PxVec3 half_size = geometry.box().halfExtents;
		glScalef(half_size.x, half_size.y, half_size.z);
		glutSolidCube(2.f);
	}

	void GlutRenderer::DrawCapsule(const physx::PxGeometryHolder& geometry) const
	{
		const physx::PxF32 radius = geometry.capsule().radius;
		const physx::PxF32 halfHeight = geometry.capsule().halfHeight;

		// Sphere
		glPushMatrix();
		glTranslatef(halfHeight, 0.f, 0.f);
		glutSolidSphere(radius, m_renderDetail, m_renderDetail);
		glPopMatrix();

		// Sphere
		glPushMatrix();
		glTranslatef(-halfHeight, 0.f, 0.f);
		glutSolidSphere(radius, m_renderDetail, m_renderDetail);
		glPopMatrix();

		// Cylinder
		glPushMatrix();
		glTranslatef(-halfHeight, 0.f, 0.f);
		glRotatef(90.f, 0.f, 1.f, 0.f);

		GLUquadric* qobj = gluNewQuadric();
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluCylinder(qobj, radius, radius, halfHeight * 2.f, m_renderDetail, m_renderDetail);
		gluDeleteQuadric(qobj);
		glPopMatrix();
	}

	void GlutRenderer::DrawConvexMesh(const physx::PxGeometryHolder& geometry) const
	{
		physx::PxConvexMesh* mesh = geometry.convexMesh().convexMesh;
		physx::PxU32 num_polys = mesh->getNbPolygons();
		const physx::PxVec3* verts = mesh->getVertices();
		const physx::PxU8* indicies = mesh->getIndexBuffer();

		for (physx::PxU32 i = 0; i < num_polys; i++)
		{
			physx::PxHullPolygon face;
			if (mesh->getPolygonData(i, face))
			{
				glBegin(GL_POLYGON);
				glNormal3f(face.mPlane[0], face.mPlane[1], face.mPlane[2]);
				const physx::PxU8* faceIdx = indicies + face.mIndexBase;

				for (physx::PxU32 j = 0; j < face.mNbVerts; j++)
				{
					physx::PxVec3 v = verts[faceIdx[j]];
					glVertex3f(v.x, v.y, v.z);
				}
				glEnd();
			}
		}
	}

	void GlutRenderer::DrawTriangleMesh(const physx::PxGeometryHolder& geometry) const
	{
		physx::PxTriangleMesh* mesh = geometry.triangleMesh().triangleMesh;
		const physx::PxVec3* verts = mesh->getVertices();

		physx::PxU16* trigs = (physx::PxU16*) mesh->getTriangles();
		const physx::PxU32 num_trigs = mesh->getNbTriangles();

		for (physx::PxU32 i = 0; i < num_trigs * 3; i += 3)
		{
			physx::PxVec3 v0 = verts[trigs[i]];
			physx::PxVec3 v1 = verts[trigs[i + 1]];
			physx::PxVec3 v2 = verts[trigs[i + 2]];

			physx::PxVec3 n = (v1 - v0).cross(v2 - v0);
			n.normalize();

			glBegin(GL_POLYGON);
			glNormal3f(n.x, n.y, n.z);
			glVertex3f(v0.x, v0.y, v0.z);
			glVertex3f(v1.x, v1.y, v1.z);
			glVertex3f(v2.x, v2.y, v2.z);
			glEnd();
		}
	}

	void GlutRenderer::DrawHeightField(const physx::PxGeometryHolder& geometry) const
	{
		const physx::PxHeightFieldGeometry& heightFieldGeometry = geometry.heightField();
		const physx::PxHeightField* heightField = heightFieldGeometry.heightField;

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_QUADS);

		for (int i = 0; i < heightField->getNbRows() - 1; ++i)
		{
			for (int j = 0; j < heightField->getNbColumns() - 1; ++j)
			{
				float height = heightField->getHeight(i, j);

				glVertex3f(i, height, j);
				glVertex3f(i + 1, height, j);
				glVertex3f(i + 1, heightField->getHeight(i + 1, j), j);
				glVertex3f(i, heightField->getHeight(i, j + 1), j + 1);
			}
		}

		glEnd();
	}

	void GlutRenderer::RenderGeometry(const physx::PxGeometryHolder& geometry) const
	{
		switch (geometry.getType())
		{
			case physx::PxGeometryType::ePLANE:
				DrawPlane();
				break;

			case physx::PxGeometryType::eSPHERE:
				DrawSphere(geometry);
				break;

			case physx::PxGeometryType::eBOX:
				DrawBox(geometry);
				break;

			case physx::PxGeometryType::eCAPSULE:
				DrawCapsule(geometry);
				break;

			case physx::PxGeometryType::eCONVEXMESH:
				DrawConvexMesh(geometry);
				break;

			case physx::PxGeometryType::eTRIANGLEMESH:
				DrawTriangleMesh(geometry);
				break;

			case physx::PxGeometryType::eHEIGHTFIELD:
				DrawHeightField(geometry);
				break;

			default:
				break;
		}
	}

	void GlutRenderer::RenderCloth(const PhysicsEngine::ClothActor* clothActor, const RenderData& renderData) const
	{
		auto physxClothActor = (physx::PxCloth*) clothActor->GetCurrentPhysxActor();
		physx::PxClothMeshDesc meshDesc = clothActor->GetMeshDesc();

		physx::PxVec3 color = *renderData.GetColor();

		physx::PxU32 quad_count = meshDesc.quads.count;
		physx::PxU32* quads = (physx::PxU32*) meshDesc.quads.data;

		std::vector<physx::PxVec3> verts(physxClothActor->getNbParticles());
		std::vector<physx::PxVec3> norms(verts.size(), physx::PxVec3(0.f, 0.f, 0.f));

		// Get verts data
		physxClothActor->lockParticleData();
		physx::PxClothParticleData* particle_data = physxClothActor->lockParticleData();
		if (!particle_data)
		{
			return;
		}

		// copy vertex positions
		for (physx::PxU32 j = 0; j < verts.size(); j++)
		{
			physx::PxVec3 pos = particle_data->particles[j].pos;
			verts[j] = pos;
		}

		particle_data->unlock();

		for (physx::PxU32 i = 0; i < quad_count * 4; i += 4)
		{
			physx::PxVec3 v0 = verts[quads[i]];
			physx::PxVec3 v1 = verts[quads[i + 1]];
			physx::PxVec3 v2 = verts[quads[i + 2]];
			physx::PxVec3 n = -((v1 - v0).cross(v2 - v0));

			norms[quads[i]] += n;
			norms[quads[i + 1]] += n;
			norms[quads[i + 2]] += n;
			norms[quads[i + 3]] += n;
		}

		for (physx::PxU32 i = 0; i < norms.size(); i++)
		{
			norms[i].normalize();
		}

		physx::PxTransform pose = physxClothActor->getGlobalPose();
		physx::PxMat44 shapePose(pose);

		glColor4f(color.x, color.y, color.z, 1.f);

		glPushMatrix();
		glMultMatrixf((float*) &shapePose);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glVertexPointer(3, GL_FLOAT, sizeof(physx::PxVec3), &verts.front());
		glNormalPointer(GL_FLOAT, sizeof(physx::PxVec3), &norms.front());

		glDrawElements(GL_QUADS, quad_count * 4, GL_UNSIGNED_INT, quads);

		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glPopMatrix();

		if (m_showShadows)
		{	
			glPushMatrix();
			glMultMatrixf(k_shadowMat);
			glMultMatrixf((float*) &shapePose);
			glDisable(GL_LIGHTING);

			// Shadow render
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);

			glVertexPointer(3, GL_FLOAT, sizeof(physx::PxVec3), &verts.front());
			glNormalPointer(GL_FLOAT, sizeof(physx::PxVec3), &norms.front());

			float originYPos = pose.p.y;

			for (int i = 0; i < quad_count; i++)
			{
				float quadCenterY = 0;

				for (int j = 0; j < 4; j++)
				{
					int vertexIndex = quads[i * 4 + j];
					quadCenterY += abs((abs(verts[vertexIndex].y) - originYPos));
				}
				quadCenterY /= 4;

				if (quadCenterY > k_specialMinRenderDistance)
				{
					float shadowHardness = (m_maxShadowDistance - quadCenterY) / m_maxShadowDistance;
					glColor4f(k_shadowColor.x, k_shadowColor.y, k_shadowColor.z, shadowHardness);
					glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, &quads[i]);
				}
			}

			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

			glEnable(GL_LIGHTING);
			glPopMatrix();
		}
	}

	void GlutRenderer::RenderBuffer(float* pVertList, float* pColorList, int type, int num) const
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, pVertList);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, 0, pColorList);
		glDrawArrays(type, 0, num);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void GlutRenderer::UpdateCameraRender(const physx::PxVec3& cameraEye, const physx::PxVec3& cameraDir) const
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.f,
					   (float) glutGet(GLUT_WINDOW_WIDTH) /
					   (float) glutGet(GLUT_WINDOW_HEIGHT),
					   1.f, 10000.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(cameraEye.x, cameraEye.y, cameraEye.z,
				  cameraEye.x + cameraDir.x, cameraEye.y + cameraDir.y, cameraEye.z + cameraDir.z,
				  0.f, 1.f, 0.f);
	}

	void GlutRenderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		UpdateCameraRender(m_camera->GetPos(), m_camera->GetDir());
	}

	void GlutRenderer::Render(const GameObject** gameActors, const physx::PxU32 numActors)
	{
		for (physx::PxU32 i = 0; i < numActors; i++)
		{
			PhysicsEngine::Actor* actor = (PhysicsEngine::Actor*) (*gameActors[i]->GetPhysicsActorPointer());
			const RenderData& renderData = gameActors[i]->GetRenderData();

			if (actor->GetType() == PhysicsEngine::ActorType::Cloth)
			{
				RenderCloth((PhysicsEngine::ClothActor*) actor, renderData);
				continue;
			}

			if (actor->GetType() == PhysicsEngine::ActorType::Static || actor->GetType() == PhysicsEngine::ActorType::Dynamic)
			{
				RenderShapes(actor, renderData);
				continue;
			}

			std::printf("Missing implementation for rendering!");
		}
	}

	void GlutRenderer::RenderShapes(PhysicsEngine::Actor* actor, const RenderData& renderData) const
	{
		physx::PxRigidActor* rigidActor = (physx::PxRigidActor*) actor->GetCurrentPhysxActor();
		if (!rigidActor)
		{
			std::printf("PxActor cast to PxRigidActor failed!");
			return;
		}

		std::vector<physx::PxShape*> shapes(rigidActor->getNbShapes());
		rigidActor->getShapes(&shapes.front(), shapes.size());

		for (physx::PxU32 j = 0; j < shapes.size(); j++)
		{
			const physx::PxShape* shape = shapes[j];
			physx::PxTransform pose = physx::PxShapeExt::getGlobalPose(*shape, *shape->getActor());
			physx::PxGeometryHolder h = shape->getGeometry();

			// Move the plane slightly down to avoid visual artefacts
			if (h.getType() == physx::PxGeometryType::ePLANE)
			{
				pose.q *= physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.f, 0.f, 1.f));
				pose.p += physx::PxVec3(0, -0.01, 0);
			}

			physx::PxMat44 shapePose(pose);
			glPushMatrix();
			glMultMatrixf((float*) &shapePose);

			// Render object
			physx::PxVec3 shapeColor = *renderData.GetColor();
			glColor4f(shapeColor.x, shapeColor.y, shapeColor.z, 1.f);
			RenderGeometry(h);
			glPopMatrix();

			// Draw shadows
			float absVerticalPos = abs(pose.p.y);
			if (m_showShadows && absVerticalPos < m_maxShadowDistance)
			{
				if (h.getType() == physx::PxGeometryType::ePLANE && absVerticalPos < k_specialMinRenderDistance)
				{
					return;
				}

				float shadowHardness = (m_maxShadowDistance-absVerticalPos) / m_maxShadowDistance;

				glPushMatrix();
				glMultMatrixf(k_shadowMat);
				glMultMatrixf((float*) &shapePose);
				glDisable(GL_LIGHTING);

				glColor4f(k_shadowColor.x, k_shadowColor.y, k_shadowColor.z, shadowHardness);
				RenderGeometry(h);

				glEnable(GL_LIGHTING);
				glPopMatrix();
			}
		}
	}

	void GlutRenderer::RenderDataBuffer(const physx::PxRenderBuffer& data, physx::PxReal line_width)
	{
		glLineWidth(line_width);

		// Render points
		unsigned int NbPoints = data.getNbPoints();

		if (NbPoints)
		{
			std::vector<float> pVertList(NbPoints * 3);
			std::vector<float> pColorList(NbPoints * 4);
			int vertIndex = 0;
			int colorIndex = 0;
			const physx::PxDebugPoint* Points = data.getPoints();

			while (NbPoints--)
			{
				pVertList[vertIndex++] = Points->pos.x;
				pVertList[vertIndex++] = Points->pos.y;
				pVertList[vertIndex++] = Points->pos.z;
				pColorList[colorIndex++] = (float) ((Points->color >> 16) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float) ((Points->color >> 8) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float) (Points->color & 0xff) / 255.f;
				pColorList[colorIndex++] = 1.f;
				Points++;
			}

			RenderBuffer(&pVertList.front(), &pColorList.front(), GL_POINTS,
						 data.getNbPoints());
		}

		// Render lines
		unsigned int NbLines = data.getNbLines();
		if (NbLines)
		{
			std::vector<float> pVertList(NbLines * 3 * 2);
			std::vector<float> pColorList(NbLines * 4 * 2);

			int vertIndex = 0;
			int colorIndex = 0;

			const physx::PxDebugLine* Lines = data.getLines();

			while (NbLines--)
			{
				pVertList[vertIndex++] = Lines->pos0.x;
				pVertList[vertIndex++] = Lines->pos0.y;
				pVertList[vertIndex++] = Lines->pos0.z;
				pColorList[colorIndex++] = (float) ((Lines->color0 >> 16) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float) ((Lines->color0 >> 8) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float) (Lines->color0 & 0xff) / 255.f;
				pColorList[colorIndex++] = 1.f;

				pVertList[vertIndex++] = Lines->pos1.x;
				pVertList[vertIndex++] = Lines->pos1.y;
				pVertList[vertIndex++] = Lines->pos1.z;
				pColorList[colorIndex++] = (float) ((Lines->color1 >> 16) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float) ((Lines->color1 >> 8) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float) (Lines->color1 & 0xff) / 255.f;
				pColorList[colorIndex++] = 1.f;

				Lines++;
			}

			RenderBuffer(&pVertList.front(), &pColorList.front(), GL_LINES,
						 data.getNbLines() * 2);
		}

		// Render triangles
		unsigned int NbTris = data.getNbTriangles();
		if (NbTris)
		{
			std::vector<float> pVertList(NbTris * 3 * 3);
			std::vector<float> pColorList(NbTris * 4 * 3);

			int vertIndex = 0;
			int colorIndex = 0;

			const physx::PxDebugTriangle* Triangles = data.getTriangles();

			while (NbTris--)
			{
				pVertList[vertIndex++] = Triangles->pos0.x;
				pVertList[vertIndex++] = Triangles->pos0.y;
				pVertList[vertIndex++] = Triangles->pos0.z;

				pVertList[vertIndex++] = Triangles->pos1.x;
				pVertList[vertIndex++] = Triangles->pos1.y;
				pVertList[vertIndex++] = Triangles->pos1.z;

				pVertList[vertIndex++] = Triangles->pos2.x;
				pVertList[vertIndex++] = Triangles->pos2.y;
				pVertList[vertIndex++] = Triangles->pos2.z;

				pColorList[colorIndex++] =
					(float) ((Triangles->color0 >> 16) & 0xff) / 255.f;
				pColorList[colorIndex++] =
					(float) ((Triangles->color0 >> 8) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float) (Triangles->color0 & 0xff) / 255.f;
				pColorList[colorIndex++] = 1.f;

				pColorList[colorIndex++] =
					(float) ((Triangles->color1 >> 16) & 0xff) / 255.f;
				pColorList[colorIndex++] =
					(float) ((Triangles->color1 >> 8) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float) (Triangles->color1 & 0xff) / 255.f;
				pColorList[colorIndex++] = 1.f;

				pColorList[colorIndex++] =
					(float) ((Triangles->color2 >> 16) & 0xff) / 255.f;
				pColorList[colorIndex++] =
					(float) ((Triangles->color2 >> 8) & 0xff) / 255.f;
				pColorList[colorIndex++] = (float) (Triangles->color2 & 0xff) / 255.f;
				pColorList[colorIndex++] = 1.f;

				Triangles++;
			}

			RenderBuffer(&pVertList.front(), &pColorList.front(), GL_TRIANGLES,
						 data.getNbTriangles() * 3);
		}

		// TODO: render texts ?
	}

	void GlutRenderer::FinishRender()
	{
		glutSwapBuffers();
	}

	bool GlutRenderer::Init(const char* name, int width, int height)
	{
		m_renderDetail = 10;
		m_maxShadowDistance = 20.f;
		m_showShadows = true;

		SetupWindow(name, width, height);

		SetupRenderStates();
		SetupLighting();

		return true;
	}

	void GlutRenderer::PostInit(const Camera* camera,
								const physx::PxVec3* backgroundColor)
	{
		m_camera = camera;
		m_backgroundColour = backgroundColor;
	}

	void GlutRenderer::Reshape(int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void GlutRenderer::Idle()
	{
		glClearColor(m_backgroundColour->x, m_backgroundColour->y, m_backgroundColour->z, 1.f);
		glutPostRedisplay();
	}
}